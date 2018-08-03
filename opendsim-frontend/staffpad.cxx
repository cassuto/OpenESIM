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

#include <frontend/error.h>

#include "Schemaview.h"

#include "staffpad.h"

namespace dsim
{

StaffPad::StaffPad( int index, const QPointF &pos, SchemaScene *schgraph, QGraphicsItem *parent, StaffEvents* events )
          : StaffGraphItem( index, schgraph, parent, events )
{
  setPos( pos );
  setBoundingRect( QRect( 0, 0, 4, 4 ) );
}

StaffPad::~StaffPad()
{
}

void StaffPad::paint( QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget )
{
  QBrush brush = painter->brush();
  brush.setStyle( Qt::SolidPattern );
  brush.setColor( Qt::red );
  painter->setBrush( brush );
  painter->setPen( Qt::red );

  painter->drawRect( boundingRect() );

  UNUSED(option); UNUSED(widget);
}

}
