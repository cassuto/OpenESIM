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

#include <cstring>
#include "templatestyle.h"
#include "elementjoint.h"

namespace dsim
{

DECLARE_ELEMENT_CAST(ElementJointPort, "jointport")

ElementJointPort::ElementJointPort( int id, SchemaScene *scene )
                 : ElementBase( id, scene )
{
}

ElementJointPort::~ElementJointPort()
{
}

void ElementJointPort::setScenePos( const QPointF &pos )
{ m_scenePos = pos; }

QPointF ElementJointPort::portScenePos() const
{ return m_scenePos; }

ElementJoint::ElementJoint( const QPointF &pos, int id, SchemaScene *scene, QGraphicsItem* parent )
             : ElementGraphItem( id, scene, /*editable*/ false, parent )
{
  moveJoint( pos );
  std::memset( m_ports, 0, sizeof(m_ports) );
}

void ElementJoint::setPorts( ElementJointPort **ports )
{
  for( int i=0; i<4; i++ ) m_ports[i] = ports[i];
}

void ElementJoint::moveJoint( const QPointF &pos )
{ setRect( QRect( -2 + pos.x(), -2 + pos.y(), 4, 4 ) ); }

int ElementJoint::serialize( LispDataset *dataset )
{
  int rc = ElementGraphItem::serialize( dataset );          UPDATE_RC(rc);

  rc = dataset->ser( double(rect().x()) );                  UPDATE_RC(rc);
  rc = dataset->ser( double(rect().y()) );                  UPDATE_RC(rc);
  rc = dataset->ser( double(rect().width()) );              UPDATE_RC(rc);
  rc = dataset->ser( double(rect().height()) );             UPDATE_RC(rc);

  for( int i=0; i<4; i++ )
    {
      if( 0l==m_ports[i] ) return -DS_FAULT;
      rc = dataset->ser( m_ports[i]->parentId() );          UPDATE_RC(rc);
    }

  return rc;
}

int ElementJoint::deserialize( LispDataset *dataset )
{
  int rc = ElementGraphItem::deserialize( dataset );        UPDATE_RC(rc);

  double x, y, width, height;

  rc = dataset->des( x );                                   UPDATE_RC(rc);
  rc = dataset->des( y );                                   UPDATE_RC(rc);
  rc = dataset->des( width );                               UPDATE_RC(rc);
  rc = dataset->des( height );                              UPDATE_RC(rc);

  for( int i=0; i<4; i++ )
    {
      rc = dataset->des( m_portIds[i] );                    UPDATE_RC(rc);
    }

  return rc;
}

int ElementJoint::resolveSubElements()
{
  for( int i=0; i<4; i++ )
    {
      ElementBase *portBase = view()->element( m_portIds[i] );
      if( 0l==portBase )
        return -DS_INVALID_ELEMENT_ID;

      ElementJointPort *port = element_cast<ElementJointPort *>(portBase);
      if( 0l==port )
        return -DS_INVALID_ELEMENT_ID;
      m_ports[i] = port;
    }
  return 0;
}

void ElementJoint::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
  Templatestyle::apply( this, 0l, 0l, "joint", isSelected() );

  QGraphicsEllipseItem::paint( painter, option, widget );
}

}
