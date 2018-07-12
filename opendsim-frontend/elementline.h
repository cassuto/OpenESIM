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

#ifndef ELEMENTLINE_H_
#define ELEMENTLINE_H_

#include <QtWidgets>

#include "elementgraphitem.h"
#include "staffgraphitem.h"

namespace dsim
{

class StaffPad;

class ElementLine : public ElementGraphItem<QGraphicsItem>, public StaffEvents
{
public:
  ElementLine( const QPointF &p0, int id, SchemaGraph *scene, bool edit, QGraphicsItem* parent = 0 );
  ~ElementLine();

  const char *classname() { return "line"; }

  void setStyle( const char *style );
  void addPoint( const QPointF &point );
  void setLastPoint( const QPointF &point );
  void removeLastPoint();
  int pointCount();

  int serialize( LispDataset *dataset );
  int deserialize( LispDataset *dataset );

  void setSelected( bool selected );

  QRectF boundingRect() const;
  QPainterPath shape() const;

protected:
  void staffMoveEvent( int index, QGraphicsSceneMouseEvent *event );
  void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0 );

private:
  void updatePads();

  QPointF m_lastpoint;

  QList<StaffPad *> m_pads;
  QList<QGraphicsLineItem *> m_lines;
};

}

#endif
