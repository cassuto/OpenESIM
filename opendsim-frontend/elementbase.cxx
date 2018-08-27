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
#include <frontend/error.h>

#include "lispdataset.h"
#include "schemaview.h"
#include "elementbase.h"

namespace dsim
{

ElementBase::ElementBase( int id, SchemaScene *scene )
            : m_refcount( 0 )
            , m_id( id )
            , m_schemaGraph( scene )
            , m_graphicsItem( 0l )
            , m_root( true )
#ifdef CHECK_ELEMENTBASE_MAGIC
            , m_magic( ELEMENTBASE_MAGIC )
#endif
{
}

ElementBase::~ElementBase()
{
  trace_assert( m_refcount == 0 );

#ifdef CHECK_ELEMENTBASE_MAGIC
  m_magic = 0xbadbeeef; // destroy the magic number
#endif
}

/*
 * The scope of reference counter is merely among schematic elements, in other words, even SchemaScene that is
 * not a schematic element can't make it sense.
 */
bool ElementBase::ref()
{ return( m_refcount++ == 0 ); }

void ElementBase::release()
{
#ifdef CHECK_ELEMENTBASE_MAGIC
  trace_assert( m_magic == ELEMENTBASE_MAGIC );
#endif
  trace_assert( m_refcount > 0 ); if( --m_refcount==0 ) delete this;
}

/*
 * To make things clearly, a root element must meet the following two conditions:
 * 1) Not any element has ever referenced this element before.
 * 2) The element shouldn't be set non-root mark.
 *
 * There is an example shows what kind of element mustn't be a root.
 *   Wires, Joints, etc. ( connected around the root )
 *
 * When you want to delete a branch of elements, consider the root element first please because the root has responsibility to delete its
 * children element. Never delete a non-root element directly as it may destroys the relationship among non-roots and root.
 */
bool ElementBase::isRoot()
{ return m_refcount == 0 && m_root; }

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

void ElementBase::move( QPointF delta ) { UNUSED(delta); }

int ElementBase::addElement( ElementBase *element )
{
  if( !element->ref() )
    {
      // trace_debug(("referenced element: %p %s\n", element, element->classname() ));
    }
  m_elements.append( element );
  return 0;
}

int ElementBase::resolveSubElements()
{
  if( m_elementIds.empty() ) return 0;

  foreach( int id, m_elementIds )
    {
      ElementBase *element = view()->element( id );
      if( element )
        {
          int rc = addElement( element ); UPDATE_RC(rc);
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

void ElementBase::releaseSubElements()
{
  foreach( ElementBase *element, elements() )
    {
      view()->releaseElement( element );
    }
}

}
