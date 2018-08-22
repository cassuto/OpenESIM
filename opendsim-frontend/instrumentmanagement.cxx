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

#include <instrument/libentry.h>
#include <instrument/instrumentbase.h>
#include <frontend/error.h>
#include <frontend/instrument-lib.h>

#include "instrumentmanagement.h"

namespace dsim
{

InstrumentManagement::InstrumentManagement() {}

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

void InstrumentManagement::clockTick()
{
  foreach( InstrumentPair pair, m_insts ) pair.base->clockTick();
}

}
