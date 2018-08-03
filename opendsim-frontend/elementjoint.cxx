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
                 , m_elementJoint( 0l )
{
  setNonRoot( true );
}

ElementJointPort::~ElementJointPort()
{
}

void ElementJointPort::setScenePos( const QPointF &pos )
{ m_scenePos = pos; }

QString ElementJointPort::reference() const
{ return "jointport"; }

QPointF ElementJointPort::portScenePos() const
{ return m_scenePos; }

void ElementJointPort::setElementJoint( ElementJoint *joint )
{ m_elementJoint = joint; }

void ElementJointPort::disconnectedEvent()
{ if( m_elementJoint ) m_elementJoint->remove(); }

ElementJoint::ElementJoint( const QPointF &pos, int id, SchemaScene *scene, QGraphicsItem* parent )
             : ElementGraphItem( id, scene, /*editable*/ false, parent )
{
  setNonRoot( true );
  setFlag( QGraphicsItemGroup::ItemIsSelectable, false );
  moveJoint( pos );
}

int ElementJoint::setPorts( ElementJointPort **ports )
{
  trace_assert( elements().count() == 0 );
  int rc, i;
  for( i=0; i<3; i++ )
    {
      rc = addElement( ports[i] ); UPDATE_RC(rc);
      ports[i]->setElementJoint( this ); // render equal ownership
    }
  return rc;
}

void ElementJoint::moveJoint( const QPointF &pos )
{
  setRect( QRect( -2 + pos.x(), -2 + pos.y(), 4, 4 ) );

  if( elements().size() )
    {
      foreach( ElementBase *element, elements() )
        {
          ElementJointPort *port = static_cast<ElementJointPort *>(element);
          port->setScenePos( pos );
        }
    }
}

ElementJointPort *ElementJoint::port( int index )
{ trace_assert( elements().count() == 3 ); return static_cast<ElementJointPort *>(elements().at( index )); }

void ElementJoint::remove()
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
      current->setElementJoint( 0l ); // deprive ownership
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

  ElementAbstractPort *wire0_startPort = wire0->startPort();
  ElementAbstractPort *wire1_endPort = wire1->endPort();

  ElementAbstractPort *startPort = port0->oppositePort();
  ElementAbstractPort *endPort = port1->oppositePort();

  wire0->disconnectPort( wire0->startPort(), /*boardcast*/ false );
  wire0->disconnectPort( wire0->endPort(), /*boardcast*/ false );
  wire1->disconnectPort( wire1->startPort(), /*boardcast*/ false );
  wire1->disconnectPort( wire1->endPort(), /*boardcast*/ false );

  /*
   * The Id of new wire will be totally different from the old one. Luckily we only make use of element Id when serializing and deserializing.
   */
  ElementWire* wire = static_cast<ElementWire *>(view()->createElement( "wire", QPoint( 0, 0 ) ));

  trace_assert( startPort->connectedWire() == 0l );
  wire->connectStartPort( startPort );

  QList<qreal> list0 = wire0->pointList();
  QList<qreal> list1 = wire1->pointList();
  QList<qreal> plist;

  if( port0 == wire0_startPort )
    while( !list0.isEmpty() )
      {
        qreal p2 = list0.takeLast();
        plist.append(list0.takeLast());
        plist.append(p2);
      }
  else while( !list0.isEmpty() ) plist.append(list0.takeFirst());

  if( port1 == wire1_endPort )
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

  qreal p0x = startPort->portScenePos().x(); // scene position of start port
  qreal p0y = startPort->portScenePos().y();

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

  trace_assert( endPort->connectedWire() == 0l );
  wire->connectEndPort( endPort );
  wire->removeNullWires();

  trace_assert( wire0->startPort() == 0l && wire0->endPort() == 0l );
  view()->releaseElement( wire0 );

  trace_assert( wire1->startPort() == 0l && wire1->endPort() == 0l );
  view()->releaseElement( wire1 );
}

int ElementJoint::serialize( LispDataset *dataset )
{
  int rc = ElementGraphItem::serialize( dataset );          UPDATE_RC(rc);

  rc = dataset->ser( double(rect().x()+2) );                UPDATE_RC(rc);
  rc = dataset->ser( double(rect().y()+2) );                UPDATE_RC(rc);

  return rc;
}

int ElementJoint::deserialize( LispDataset *dataset )
{
  int rc = ElementGraphItem::deserialize( dataset );        UPDATE_RC(rc);

  double x, y;

  rc = dataset->des( x );                                   UPDATE_RC(rc);
  rc = dataset->des( y );                                   UPDATE_RC(rc);

  moveJoint( QPointF( x, y ) );
  return rc;
}

int ElementJoint::resolveSubElements()
{
  int rc = ElementGraphItem::resolveSubElements(); UPDATE_RC(rc);
  if( elements().count() != 3 ) return -DS_INVALID_ELEMENT_ID;

  for( int i=0; i<3; i++ )
    {
      ElementJointPort *port = element_cast<ElementJointPort *>(elements().at(i));
      if( 0l==port )
        return -DS_INVALID_ELEMENT_ID;

      port->setScenePos( QPointF( rect().x()+2, rect().y()+2 ) );
      port->setElementJoint( this ); // render equal ownership
    }
  return rc;
}

void ElementJoint::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
  Templatestyle::apply( this, 0l, 0l, "joint", isSelected() );

  QGraphicsEllipseItem::paint( painter, option, widget );
}

}
