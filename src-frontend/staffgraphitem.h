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

#ifndef SCHEMASTAFF_H_
#define SCHEMASTAFF_H_

#include <QtWidgets>

#include "schemagraph.h"

namespace dsim
{

class StaffEvents
{
public:
  virtual ~StaffEvents() {}

protected:
  virtual void staffMoveEvent( int index, QGraphicsSceneMouseEvent *event )=0;

  friend class StaffGraphItem;
};

class StaffGraphItem : public QGraphicsItem
{
public:
  StaffGraphItem( int index, SchemaGraph *schgraph, QGraphicsItem *parent = 0, StaffEvents *event = 0 );
  ~StaffGraphItem();

  virtual const char *classname() { return "none"; }

  virtual QRectF boundingRect() const { return m_area; }
  virtual void setBoundingRect( const QRect &area );
  virtual void paintBound( QPainter *painter );

  qreal width();
  qreal height();
  QPointF central();

  inline SchemaView *view() const { return m_schgraph->view(); }

protected:
  virtual void mousePressEvent( QGraphicsSceneMouseEvent *event );
  virtual void mouseMoveEvent( QGraphicsSceneMouseEvent *event );
  virtual void mouseReleaseEvent( QGraphicsSceneMouseEvent *event );

protected:
  int                       m_index;
  SchemaGraph              *m_schgraph;
  QRectF                    m_area;
  StaffEvents              *m_events;
};

}

#endif
