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

#ifndef ELEMENTRECT_H_
#define ELEMENTRECT_H_

#include "elementgraphitem.h"
#include "staffgraphitem.h"

namespace dsim
{

class StaffPad;

class ElementRect : public ElementGraphItem<QGraphicsRectItem>, public StaffEvents
{
public:
  ElementRect( const QRectF &rect, int id, SchemaScene *scene, bool edit, QGraphicsRectItem* parent = 0 );
  ~ElementRect();

  const char *classname() const { return "rect"; }

  void setStyle( const char *style );
  void setRect( const QRectF &rect );

  int serialize( LispDataset *dataset );
  int deserialize( LispDataset *dataset );

  void setSelected( bool selected );

protected:
  void staffMoveEvent( int index, bool fineTurning, QGraphicsSceneMouseEvent *event );
  void paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget = 0 );

private:
  void updatePads();

  StaffPad *m_pads[4];
};

}

#endif
