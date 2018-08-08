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

#include "staffgraphitem.h"

namespace dsim
{

StaffGraphItem::StaffGraphItem( int index, SchemaScene *schgraph, QGraphicsItem *parent, StaffEvents *events )
               : QGraphicsItem( parent )
               , m_index( index )
               , m_schgraph( schgraph )
               , m_events( events )
{
  setFlag( QGraphicsItem::ItemStacksBehindParent, false );
  setFlag( QGraphicsItem::ItemIsSelectable, false );
}

StaffGraphItem::~StaffGraphItem()
{
}

qreal StaffGraphItem::width()
{ return boundingRect().right() - boundingRect().left() - 1; }

qreal StaffGraphItem::height()
{ return boundingRect().bottom() - boundingRect().top() - 1; }

QPointF StaffGraphItem::central()
{ return QPointF( boundingRect().x() + width() / 2, boundingRect().y() + height() / 2); }

void StaffGraphItem::setBoundingRect( const QRect &area )
{ prepareGeometryChange(); m_area = area; }

void StaffGraphItem::paintBound( QPainter *painter )
{
  QPen pen = painter->pen();
  painter->setPen( QPen( Qt::red, 0.5 ) );
  painter->drawRect( boundingRect() );
  painter->setPen( pen );
}

void StaffGraphItem::mousePressEvent( QGraphicsSceneMouseEvent *event )
{
  if( event->button() == Qt::LeftButton )
    {
      event->accept();
      grabMouse();
    }
}

void StaffGraphItem::mouseMoveEvent( QGraphicsSceneMouseEvent *event )
{
  event->accept();
  bool fineTurning = ( QApplication::keyboardModifiers() == Qt::ControlModifier );
  if( m_events ) m_events->staffMoveEvent( m_index, fineTurning, event ); // notify the event receiver
}

void StaffGraphItem::mouseReleaseEvent( QGraphicsSceneMouseEvent *event )
{
  event->accept();
  ungrabMouse();
  if( m_events ) m_events->staffMouseReleaseEvent( m_index, event );
}


}
