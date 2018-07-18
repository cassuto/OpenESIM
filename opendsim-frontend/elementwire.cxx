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

#include <cmath>
#include <dsim/error.h>

#include "templatestyle.h"
#include "lispdataset.h"
#include "schemaview.h"
#include "staffpad.h"

#include "elementwire.h"

namespace dsim
{

ElementWire::ElementWire( int id, SchemaGraph *scene, QGraphicsItem* parent )
          : ElementBase( id, scene )
          , QGraphicsLineItem( parent )
{
  elementbase_register_cast( this );
}

ElementWire::~ElementWire()
{
}

void ElementWire::connectWire( ElementWire *wire )
{
  m_connectedWires.append( wire );
}

void ElementWire::disconnectWire( ElementWire *wire )
{
  m_connectedWires.removeOne( wire );
}

void ElementWire::setLine( const QLineF &line )
{
  QGraphicsLineItem::setLine( line );

  qreal dx = line.p2().x() - line.p1().x();
  qreal dy = line.p2().y() - line.p1().y();

  qreal x1, y1, x2, y2;

  if( ( dy == 0 ) && ( dx != 0 ) )
    {
      setCursor( Qt::SplitVCursor );
    }
  else if( ( dx == 0 ) && ( dy != 0 ) )
    {
      setCursor( Qt::SplitHCursor );
    }
}

void ElementWire::setP1( const QPointF &point )
{
  foreach( ElementWire *wire, m_connectedWires )
    {
      wire->setP2( point );
    }

  QLineF l = line();
  l.setP1( point );
  setLine( l );
}

void ElementWire::setP2( const QPointF &point )
{
  foreach( ElementWire *wire, m_connectedWires )
    {
      wire->setP2( point );
    }

  QLineF l = line();
  l.setP2( point );
  setLine( l );
}

void ElementWire::moveWire( const QPointF &delta )
{
  QLineF l = line();
  qreal dx = line().p2().x() - line().p1().x();
  qreal dy = line().p2().y() - line().p1().y();

  qreal x1, y1, x2, y2;

  if( ( dy == 0 ) && ( dx != 0 ) )
    {
      y1 += delta.y();
      y2 += delta.y();
    }
  else if( ( dx == 0 ) && ( dy != 0 ) )
    {
      x1 += delta.x();
      x2 += delta.x();
    }
  else return;

  foreach( ElementWire *wire, m_connectedWires )
    {
      wire->setP1( line().p2() );
      wire->setP2( line().p1() );
    }
}

void ElementWire::move( QPointF delta )
{
  moveWire( delta );
}

int ElementWire::serialize( LispDataset *dataset )
{
  int rc = ElementBase::serialize( dataset );               UPDATE_RC(rc);

  QPointF cp = pos();

  bool x0 = true;

  rc = dataset->ser( double(line().x1()) );                 UPDATE_RC(rc);
  rc = dataset->ser( double(line().y1()) );                 UPDATE_RC(rc);
  rc = dataset->ser( double(line().x2()) );                 UPDATE_RC(rc);
  rc = dataset->ser( double(line().y2()) );                 UPDATE_RC(rc);
  return rc;
}

int ElementWire::deserialize( LispDataset *dataset )
{
  int rc = ElementBase::deserialize( dataset );             UPDATE_RC(rc);

  double x1, y1, x2, y2;

  rc = dataset->des( x1 );                                  UPDATE_RC(rc);
  rc = dataset->des( y1 );                                  UPDATE_RC(rc);
  rc = dataset->des( x2 );                                  UPDATE_RC(rc);
  rc = dataset->des( y2 );                                  UPDATE_RC(rc);

  setLine( QLineF(x1, y1, x2, y2) );

  return rc;
}

void ElementWire::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
  if( event->button() == Qt::LeftButton )
    {
      grabMouse();
    }
}

void ElementWire::mouseMoveEvent( QGraphicsSceneMouseEvent *event )
{
  if( event->button() == Qt::LeftButton )
    {
      QPoint delta = togrid( event->scenePos() ).toPoint() - togrid(event->lastScenePos()).toPoint();
      moveWire( delta );
    }
}

void ElementWire::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
  if( event->button() == Qt::LeftButton )
    {
      ungrabMouse();
    }
}

void ElementWire::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
  Templatestyle::apply( painter, 0l, 0l, "wire", isSelected() );

  setPen( painter->pen() );
  QGraphicsLineItem::paint( painter, option, widget );
}

}
