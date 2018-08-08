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

#ifndef ELEMENTORIGIN_H_
#define ELEMENTORIGIN_H_

#include "elementgraphitem.h"
#include "staffgraphitem.h"

namespace dsim
{

class StaffPad;

class ElementOrigin : public ElementGraphItem<QGraphicsItem>
{
public:
  ElementOrigin( const QPointF &pos, int id, SchemaScene *scene, bool edit, QGraphicsRectItem* parent = 0 );
  ~ElementOrigin();

  const char *classname() const { return "origin"; }
  QRectF boundingRect() const;
  QPointF originPos() const;

  int serialize( LispDataset *dataset );
  int deserialize( LispDataset *dataset );

protected:
  void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0 );
};

}

#endif
