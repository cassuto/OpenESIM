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

#include "schemaview.h"
#include <cstdio>
#include "schemagraph.h"

namespace dsim
{

SchemaGraph::SchemaGraph( qreal x, qreal y, qreal width, qreal height, SchemaView*  parent)
           : QGraphicsScene( x, y, width, height, (QObject*)parent )
           , m_schemaView( parent )
{
  setObjectName( "Circuit" );
  setParent( (QObject*)parent );
  m_scenerect.setRect( x, y, width, height );
  setSceneRect( QRectF(x, y, width, height) );

  m_paintGrid = true;
}

SchemaGraph::~SchemaGraph()
{
}

void SchemaGraph::remove()
{
}

bool SchemaGraph::paintGrid()
{
    return m_paintGrid;
}
void SchemaGraph::setPaintGrid( bool paint )
{
    m_paintGrid = paint;
}

void SchemaGraph::drawBackground ( QPainter*  painter, const QRectF & rect )
{
  Q_UNUSED( rect );

  painter->setBrush(QColor( 240, 240, 210 ) );
  painter->drawRect( m_scenerect );
  painter->setPen( QColor( 210, 210, 210 ) );

  if( !m_paintGrid ) return;

  double scaleFactor = painter->matrix().m11();

  QPen pen = painter->pen();
  pen.setWidthF( pen.widthF() / scaleFactor );
  painter->setPen( pen );

  int startx = int(m_scenerect.x());
  int endx   = int(m_scenerect.width())/2;
  int starty = int(m_scenerect.y());
  int endy   = int(m_scenerect.height())/2;

  for (int i = 4; i<endx; i+=8)
    {
      painter->drawLine( i, starty, i, endy );
      painter->drawLine(-i, starty,-i, endy );
    }
  for (int i = 4; i<endy; i+=8)
    {
      painter->drawLine( startx, i, endx, i);
      painter->drawLine( startx,-i, endx,-i);
    }
}

} // namespace dsim
