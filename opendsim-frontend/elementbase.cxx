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

#define TRACE_UNIT "elementbase"
#include <dsim/logtrace.h>
#include <dsim/error.h>

#include "lispdataset.h"
#include "schemaview.h"
#include "elementbase.h"

namespace dsim
{

ElementBase::ElementBase( int id, SchemaScene *scene )
            : m_id( id )
            , m_ref( false )
            , m_schemaGraph( scene )
            , m_graphicsItem( 0l )
{
}

bool ElementBase::ref()
{ if( m_ref) return false; m_ref = true; return true; }

bool ElementBase::isRef()
{ return m_ref; }

QList<ElementBase*> &ElementBase::elements()
{ return m_elements; }

QRectF ElementBase::boundingRect() const
{ trace_assert(0); return QRectF(0,0,0,0); }

int ElementBase::serialize( LispDataset *dataset )
{
  int rc = dataset->ser( m_id );                        UPDATE_RC(rc);
      rc = dataset->ser( m_elements );                  UPDATE_RC(rc);
  return rc;
}

int ElementBase::deserialize( LispDataset *dataset )
{
  int rc = dataset->des( m_id );                        UPDATE_RC(rc);
      rc = dataset->des( m_elementIds );                UPDATE_RC(rc);

  if( !view()->idUnused( m_id ) )
    {
      m_id = 0;
      return -DS_INVALID_ELEMENT_ID;
    }
  view()->addId( m_id );
  return rc;
}

void ElementBase::move( QPointF delta ) {}

int ElementBase::addElement( ElementBase *element )
{
  if( element->ref() )
    {
      m_elements.append( element );
      return 0;
    }
  else
    {
      trace_debug(("referenced element: %p %s\n", element, element->classname() ));
      return -DS_REREFERENCE;
    }
}

int ElementBase::resolveSubElements()
{
  if( m_elementIds.empty() ) return 0;

  foreach( int id, m_elementIds )
    {
      ElementBase *element = view()->element( id );
      if( element )
        {
          if( element->ref() )
            {
              m_elements.append( element );
            }
          else
            {
              return -DS_REREFERENCE;
            }
        }
      else
        {
          return -DS_SOLVE_SUBELEMENTS;
        }
    }

  m_elementIds.clear();
  setLayout();
  return 0;
}

void ElementBase::deleteSubElements()
{
  foreach( ElementBase *element, elements() )
    {
      view()->deleteElement( element );
    }
}

}
