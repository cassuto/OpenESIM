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

#include <QPainter>

#include "templatestyle.h"
#include "mainwindow.h"
#include "elementgraphitem.h"
#include "componentgraph.h"

namespace dsim
{

ComponentGraph::ComponentGraph()
              : m_painter( 0l )
              , m_selected( false )
{
}

ComponentGraph::~ComponentGraph()
{
}

void ComponentGraph::setStyle( const char *style )
{
  Templatestyle::applyStyle( m_painter, style, m_selected );
}

void ComponentGraph::setBrushColor( int r, int g, int b )
{
  QBrush brush = m_painter->brush();
  brush.setColor( QColor(r, g, b) );
  m_painter->setBrush( brush );
}

void ComponentGraph::setBrushStyle( BrushStyle style )
{
  QBrush brush = m_painter->brush();

  brush.setStyle( Templatestyle::toQtBrushStyle( style ) );
  m_painter->setBrush( brush );
}

void ComponentGraph::setPenColor( int r, int g, int b )
{
  QPen pen = m_painter->pen();
  if( r<0 || g<0 || b<0 ) return;
  pen.setColor( QColor(r, g, b) );
  m_painter->setPen( pen );
}

void ComponentGraph::setPenWidth( float w )
{
  QPen pen = m_painter->pen();
  if( w<0 ) return;
  pen.setWidthF( w );
  m_painter->setPen( pen );
}

void ComponentGraph::setPenStyle( LineStyle style )
{
  QPen pen = m_painter->pen();

  switch( style )
  {
    case LINE_NONE: pen.setStyle( Qt::NoPen ); break;
    case LINE_SOLID: pen.setStyle( Qt::SolidLine ); break;
    case LINE_DASH: pen.setStyle( Qt::DashLine ); break;
    case LINE_DOT: pen.setStyle( Qt::DotLine ); break;
    case LINE_DASH_DOT: pen.setStyle( Qt::DashDotLine ); break;
    case LINE_DASH_DOTDOT: pen.setStyle( Qt::DashDotDotLine ); break;
    default: return;
  }

  m_painter->setPen( pen );
}

void ComponentGraph::point( int x, int y )
{
  m_painter->drawPoint( x, y );
}

void ComponentGraph::line( int x1, int y1, int x2, int y2 )
{
  m_painter->drawLine( x1, y1, x2, y2 );
}

void ComponentGraph::rect( int x, int y, int w, int h )
{
  m_painter->drawRect( x, y, w, h );
}

void ComponentGraph::roundRect( int x, int y, int w, int h, int xRnd, int yRnd )
{
  m_painter->drawRoundRect( x, y, w, h, xRnd, yRnd );
}

void ComponentGraph::ellipse( int x, int y, int w, int h )
{
  m_painter->drawEllipse( x, y, w, h );
}

void ComponentGraph::arc( int x, int y, int w, int h, int a, int alen )
{
  m_painter->drawArc( x, y, w, h, a, alen );
}

} // namespace dsim
