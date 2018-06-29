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

#include <QtGui>

#include "schemagraph.h"
#include "schemaview.h"

namespace dsim
{

SchemaView::SchemaView( QWidget *parent )
    : QGraphicsView( parent )
{
  m_schemaGraph = 0l;
  m_scalefactor = 1;

  clear();

  viewport()->setFixedSize(1600,1200);
  setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
  setVerticalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
  setViewportUpdateMode( QGraphicsView::MinimalViewportUpdate );
  setCacheMode( CacheBackground );
  setRenderHint( QPainter::Antialiasing );
  setTransformationAnchor( AnchorUnderMouse );
  setResizeAnchor( AnchorUnderMouse );
  setDragMode( QGraphicsView::RubberBandDrag );

  setAcceptDrops(true);
}
SchemaView::~SchemaView() { }

void SchemaView::clear()
{
  if( m_schemaGraph )
    {
      m_schemaGraph->remove();
      m_schemaGraph->deleteLater();
    }
  m_schemaGraph = new SchemaGraph( -1600, -1200, 3200, 2400, this );
  setScene( m_schemaGraph );
  centerOn( 300, 200 );
}

void SchemaView::wheelEvent(QWheelEvent *event)
{
  scaleView(pow( 2.0, event->delta() / 700.0));
}

void SchemaView::scaleView(qreal scaleFactor)
{
  qreal factor = matrix().scale(scaleFactor, scaleFactor).mapRect(QRectF(0, 0, 8, 8)).width();

  if( factor < 2 || factor > 100 ) return;

  m_scalefactor *=  factor;
  scale(scaleFactor, scaleFactor);
}

void SchemaView::resizeEvent( QResizeEvent *event )
{
  int width = event->size().width();
  int height = event->size().height();

  m_schemaGraph->setSceneRect( -width/2+2, -height/2+2, width-4, height-4 );

  QGraphicsView::resizeEvent(event);
}

void SchemaView::keyPressEvent( QKeyEvent *event )
{
  if( event->key() == Qt::Key_Shift )
    setDragMode( QGraphicsView::ScrollHandDrag );
}

void SchemaView::keyReleaseEvent( QKeyEvent *event )
{
  if( event->key() == Qt::Key_Shift )
    setDragMode( QGraphicsView::RubberBandDrag );
}

} // namespace dsim
