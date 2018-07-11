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
#include <dsim/device-lib.h>
#include <dsim/error.h>

#include "schemagraph.h"
#include "elementgraphitem.h"
#include "componentgraph.h"
#include "componentgraphitem.h"

namespace dsim
{

ComponentGraphItem::ComponentGraphItem( const DeviceLibraryEntry *deviceEntry, int id, SchemaGraph *scene, bool edit, QGraphicsItem *parent )
                  : ElementGraphItem<QGraphicsItem>( id, scene, edit, parent )
                  , m_device( 0l )
                  , m_deviceEntry( deviceEntry )
                  , m_deviceGraph( new ComponentGraph() )
{
#if 0
  IRECT boundRect = device->get_bound();
  setBoundingRect( QRect( boundRect.x, boundRect.y, boundRect.w, boundRect.h ) );
#endif

  QGraphicsItem::setFlag( QGraphicsItem::ItemIsSelectable, true );
}

ComponentGraphItem::~ComponentGraphItem()
{
  delete m_deviceGraph;
}

void ComponentGraphItem::setBoundingRect( const QRect &bounding )
{ prepareGeometryChange(); m_bounding = bounding; }

int ComponentGraphItem::serialize( LispDataset *dataset )
{
  int rc = ElementGraphItem<QGraphicsItem>::serialize( dataset ); UPDATE_RC(rc);

  std::string symbol = m_deviceEntry->symbol_name;

  rc = dataset->ser( double(pos().x()) );                   UPDATE_RC(rc);
  rc = dataset->ser( double(pos().y()) );                   UPDATE_RC(rc);
  rc = dataset->ser( symbol );                              UPDATE_RC(rc);

  return rc;
}

int ComponentGraphItem::deserialize( LispDataset *dataset )
{
  int rc = ElementGraphItem<QGraphicsItem>::deserialize( dataset ); UPDATE_RC(rc);

  double x, y;
  std::string symbol;

  rc = dataset->des( x );                                   UPDATE_RC(rc);
  rc = dataset->des( y );                                   UPDATE_RC(rc);
  rc = dataset->des( symbol );                              UPDATE_RC(rc);

  setPos( QPointF( x, y ) );

  m_deviceEntry = device_lib_entry( symbol.c_str() );

  return 0l==m_deviceEntry ? -DS_INVALID_ELEMENT_SYMBOL : rc;
}

void ComponentGraphItem::paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget )
{
  Q_UNUSED(option); Q_UNUSED(widget);

  QPen pen(QColor( 0, 0, 128 ), 1.0f, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);

  if ( QGraphicsItem::isSelected() )
    {
      pen.setColor( Qt::red);
    }

  painter->setPen( pen );

  m_deviceGraph->setPainter( painter );

  m_device->render_frame( m_deviceGraph );
}

}
