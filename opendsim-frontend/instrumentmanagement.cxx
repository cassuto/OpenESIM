/*
 *  OpenDSIM (A/D mixed circuit simulator)
 *  Copyleft (C) 2016, The first Middle School in Yongsheng Lijiang China
 *
 *  This project is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License(GPL)
 *  as published by the Free Software Foundation; either version 2.1
 *  of the License, or (at your option) any later version.
 *
 *  This project is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 */

#define TRACE_UNIT "instman"
#include <dsim/logtrace.h>

#include <device/device.h>
#include <instrument/libentry.h>
#include <instrument/instrumentbase.h>
#include <frontend/error.h>
#include <frontend/instrument-lib.h>

#include "lispdataset.h"
#include "componentgraphitem.h"
#include "schemaview.h"
#include "instrumentmanagement.h"

namespace dsim
{

InstrumentManagement::InstrumentManagement() : m_schemaView( 0l ) {}

int InstrumentManagement::addInstrument( InstrumentLibraryEntry *entry, InstrumentBase DS_OUT **ppInstance )
{
  if( entry )
    {
      if( InstrumentBase *instance = entry->construct( /*index*/m_insts.count() ) )
        {
          *ppInstance = instance;
          m_insts.append( InstrumentPair( entry, instance ) );
          return 0;
        }
    }
  return -DS_CREATE_INSTRUMENT;
}

int InstrumentManagement::addInstrument( const char *classname, InstrumentBase DS_OUT **ppInstance )
{ return addInstrument( instrument_lib_entry( classname ), ppInstance ); }

void InstrumentManagement::removeInstrument( const InstrumentPair &pair )
{
  int index = m_insts.indexOf( pair );
  if( index > -1 )
    {
      m_insts.removeAt( index );
      for( int i=index; i<m_insts.count(); i++ )
        {
          m_insts.at(i).base->setIndex( i );
        }
    }
}

rb_tree_t *InstrumentManagement::instrumentTree() const
{ return instrument_lib_get_tree(); }

int InstrumentManagement::clockTick()
{
  int rc;
  foreach( InstrumentPair pair, m_insts )
    if( (rc = pair.base->clockTick()) )
      return rc;
  return 0;
}

int InstrumentManagement::serialize( LispDataset *dataset )
{
  int rc;

  rc = dataset->beginEntry( "insts" );                                  UPDATE_RC(rc);

  foreach( InstrumentPair pair, m_insts )
    {
      if( pair.base->probeDevice()->valid() )
        {
          rc = dataset->beginNode( false, pair.entry->classname );           UPDATE_RC(rc);
          rc = dataset->ser( pair.base->index() );                          UPDATE_RC(rc);
          rc = dataset->ser( pair.base->probeDevice()->get()->id() );       UPDATE_RC(rc);
          rc = dataset->endNode();                                          UPDATE_RC(rc);
        }
    }

  rc = dataset->endEntry();                                             UPDATE_RC(rc);

  return 0;
}

int InstrumentManagement::deserialize( LispDataset *dataset )
{
  int rc;
  std::string classname;
  int index, probeId;

  DomEntry nodes = dataset->entry( 0, "insts" );
    {
      for( nodes.begin(); nodes.valid(); ++nodes )
        {
          dataset->deserializePush( nodes.child() );
          {
            LispDataset::AutoPop autoPop( dataset );

            rc = dataset->des( classname, /*symbol*/true );             UPDATE_RC(rc);
            rc = dataset->des( index );                                 UPDATE_RC(rc);
            rc = dataset->des( probeId );                               UPDATE_RC(rc);

            InstrumentBase *instrument = 0l;
            rc = addInstrument( classname.c_str(), &instrument );       UPDATE_RC(rc);

            ElementBase *element = schemaView()->element( probeId );
            if( element && 0==std::strcmp( element->classname(), "component" ) )
              {
                ComponentGraphItem *component = static_cast<ComponentGraphItem *>(element);
                instrument->setProbeDevice( component->device() );
              }

            instrument->setIndex( index );

          } // auto push pop
        }
    }
  return 0;
}

}
