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

#ifndef ELEMENTWIRE_H_
#define ELEMENTWIRE_H_

#include <QtWidgets>

#include "elementbase.h"

namespace dsim
{

class ElementWire : public ElementBase, public QGraphicsLineItem
{
public:
  ElementWire( int id, SchemaGraph *scene, QGraphicsItem* parent = 0 );
  ~ElementWire();

  const char *classname() { return "wire"; }

  void connectWire( ElementWire *wire );
  void disconnectWire( ElementWire *wire );
  void setLine( const QLineF &line );
  void setP1( const QPointF &point );
  void setP2( const QPointF &point );
  void moveWire( const QPointF &delta );
  void move( QPointF delta );

  int serialize( LispDataset *dataset );
  int deserialize( LispDataset *dataset );

protected:
  void mousePressEvent( QGraphicsSceneMouseEvent *event );
  void mouseMoveEvent( QGraphicsSceneMouseEvent *event );
  void mouseReleaseEvent( QGraphicsSceneMouseEvent *event );
  void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0 );

private:
  QList<ElementWire *> m_connectedWires;
};

}

#endif
