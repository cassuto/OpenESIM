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

#define TRACE_UNIT "elementjoint"
#include <dsim/logtrace.h>
#include <cstring>
#include "templatestyle.h"
#include "elementpin.h"
#include "elementwire.h"
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
}

int ElementJoint::setPorts( ElementJointPort **ports )
{
  trace_assert( elements().count() == 0 );
  int rc;
  for( int i=0; i<3; i++ )
    {
      rc = addElement( ports[i] ); UPDATE_RC(rc);
    }
  return rc;
}

void ElementJoint::moveJoint( const QPointF &pos )
{ setRect( QRect( -2 + pos.x(), -2 + pos.y(), 4, 4 ) ); }

ElementJointPort *ElementJoint::port( int index )
{ trace_assert( elements().count() == 3 ); return static_cast<ElementJointPort *>(elements().at( index )); }

void ElementJoint::release()
{
  ElementJointPort *port[2] = { 0l, 0l };
  int nwires = 0;
  int nconecteds = 0;

  for( int i=0; i<3; i++)
    {
      ElementJointPort *current = static_cast<ElementJointPort *>(elements().at(i));
      if( current->connectedWire() )
        {
          if( nconecteds == 0 ) { nconecteds++; port[0] = current; }
          else port[1] = current;
          nwires++;
        }
    }

  if( nwires == 2 )
    {
      joinWires( port[0], port[1] );
      view()->releaseElement( this );
    }
}

void ElementJoint::joinWires( ElementAbstractPort* port0, ElementAbstractPort* port1 )
{
  ElementWire* wire0 = port0->connectedWire();
  ElementWire* wire1 = port1->connectedWire();
  wire0->removeNullWires();
  wire1->removeNullWires();

  int con0Id = wire0->id();
  ElementWire* wire = static_cast<ElementWire *>(view()->createElement( "wire", QPoint( 0, 0 ) ));
  wire->connectStartPort( port0->oppositePort() );

  QList<qreal> list0 = wire0->pointList();
  QList<qreal> list1 = wire1->pointList();
  QList<qreal> plist;

  if( port0 == wire0->startPort() )
    while( !list0.isEmpty() )
      {
        qreal p2 = list0.takeLast();
        plist.append(list0.takeLast());
        plist.append(p2);
      }
  else while( !list0.isEmpty() ) plist.append(list0.takeFirst());

  if( port1 == wire1->endPort() )
    while( !list1.isEmpty() )
      {
        qreal p2 = list1.takeLast();
        plist.append(list1.takeLast());
        plist.append(p2);
      }
  else while( !list1.isEmpty() ) plist.append(list1.takeFirst());

  qreal p1x = plist.first();
  qreal p1y = plist.at(1);
  qreal p2x = plist.at(plist.size()-2);
  qreal p2y = plist.last();

  qreal p0x = wire->startPort()->portScenePos().x();
  qreal p0y = wire->startPort()->portScenePos().y();

  wire->addWire( QLine( p0x, p0y, p1x, p1y ), 0 );

  int count = plist.size();
  for (int i=2; i<count; i+=2)
    {
      p2x = plist.at(i);
      p2y = plist.at(i+1);
      wire->addWire( QLine( p1x, p1y, p2x, p2y ), i/2 );
      p1x = p2x;
      p1y = p2y;
    }

  wire0->setStartPort( 0l );
  wire0->setEndPort( 0l );
  view()->releaseElement( wire0 );
  wire->resetId( con0Id ); // Replace the original ID position of wire0

  wire1->setStartPort( 0l );
  wire1->setEndPort( 0l );
  view()->releaseElement( wire1 );

  wire->connectEndPort( port1->oppositePort() );
  wire->removeNullWires();
}

int ElementJoint::serialize( LispDataset *dataset )
{
  int rc = ElementGraphItem::serialize( dataset );          UPDATE_RC(rc);

  rc = dataset->ser( double(rect().x()) );                  UPDATE_RC(rc);
  rc = dataset->ser( double(rect().y()) );                  UPDATE_RC(rc);
  rc = dataset->ser( double(rect().width()) );              UPDATE_RC(rc);
  rc = dataset->ser( double(rect().height()) );             UPDATE_RC(rc);

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

  return rc;
}

int ElementJoint::resolveSubElements()
{
  if( elements().count() != 3 ) return -DS_INVALID_ELEMENT_ID;

  for( int i=0; i<3; i++ )
    {
      ElementJointPort *port = element_cast<ElementJointPort *>(elements().at(i));
      if( 0l==port )
        return -DS_INVALID_ELEMENT_ID;
    }
  return 0;
}

void ElementJoint::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
  Templatestyle::apply( this, 0l, 0l, "joint", isSelected() );

  QGraphicsEllipseItem::paint( painter, option, widget );
}

}
