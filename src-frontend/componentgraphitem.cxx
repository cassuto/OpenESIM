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

#include <device/device.h>
#include <device/graph.h>

#include "schemagraph.h"
#include "elementgraphitem.h"
#include "componentgraph.h"
#include "componentgraphitem.h"

namespace dsim
{

ComponentGraphItem::ComponentGraphItem( int id, bool edit, IDevice *device, SchemaGraph *schgraph, QGraphicsItem *parent )
                  : ElementGraphItem( id, edit, schgraph, parent )
                  , m_device( device )
                  , m_deviceGraph( new ComponentGraph() )
                  , m_schgraph( schgraph )
{
  IRECT boundRect = device->get_bound();
  setBoundingRect( QRect( boundRect.x, boundRect.y, boundRect.w, boundRect.h ) );

  setFlag( QGraphicsItem::ItemIsSelectable, true );
}

ComponentGraphItem::~ComponentGraphItem()
{
  delete m_deviceGraph;
}

void ComponentGraphItem::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
  Q_UNUSED(option); Q_UNUSED(widget);

  QPen pen(QColor( 0, 0, 128 ), 1.0f, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

  if ( this->isSelected() )
    {
      pen.setColor( Qt::red);
    }

  painter->setPen( pen );

  m_deviceGraph->setPainter( painter );

  m_device->render_frame( m_deviceGraph );
}

}
