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

#include "plotterrenderer.h"

namespace dsim
{

PlotterRenderer::PlotterRenderer( int width, int height, QWidget *parent /* = 0l*/ ) : QWidget( parent )
{
  antialiased = false;

  setBackgroundRole( QPalette::Base );
  setAutoFillBackground( true );

  m_width  = width;
  m_height = height;
  setZero( 0 );
  QPen pen( QColor( 183, 183, 10 ) );
  pen.setWidth( 2 );
  setPen( pen );
  setBkcolor( QColor( 255, 255, 255 ) /*QColor( 10, 15, 50 )*/ );
  setTick( 1e6 );

  QFont font;
  font.setPointSize(7);
  pixmap = QPixmap( m_width, m_height );
  QPainter p( &pixmap );
  p.setFont( font );
  p.end();

  m_sample = 0;
  m_sampleP = 0;

  m_sec = 0;
}

QSize PlotterRenderer::minimumSizeHint() const  {  return QSize( 100, 150 );  }

QSize PlotterRenderer::sizeHint() const  { return QSize( 400, 200 ); }

void PlotterRenderer::setPen( const QPen &pen )
{ m_pen = pen; }

void PlotterRenderer::setBrush( const QBrush &brush_ )
{ brush = brush_; }

void PlotterRenderer::setBkcolor( const QColor &color )
{ m_bkcolor = color; }

void PlotterRenderer::setAntialiased( const bool antialiased_ )
{ antialiased = antialiased_; }

void PlotterRenderer::setSample( int sample )
{
  sample = (sample/2+250)*m_height/520;
  sample = m_height-sample-4;
  m_sampleP = m_sample;
  m_sample = sample;
}

void PlotterRenderer::setZero( int zero )
{
  m_zero = (zero/2+250)*m_height/520;
  m_zero = m_height-m_zero-4;
}

void PlotterRenderer::printGrid()
{
  m_sec++;

  int last = m_width-1;
  QString sec = "";

  QPainter painter( &pixmap );
  if( m_sec == 10 )
    {
     m_sec = 0;
     painter.setPen( QColor( 90, 90, 150 ) );
    }
  else painter.setPen( QColor( 50, 50, 100 ) );

  sec.setNum(m_sec);

  painter.drawLine( last, 0, last, m_height );
  painter.end();
}

void PlotterRenderer::printSample()
{
  pixmap.scroll( -1, 0, pixmap.rect() );

  QPainter painter( &pixmap );

  if( antialiased )
    {
      painter.setRenderHint( QPainter::Antialiasing, true );
      //p.translate( +0.5, +0.5 );
    }
  int last = m_width-1;

  painter.setPen( m_bkcolor );
  painter.drawLine( last, 0, last, m_height ); // erase the previous line

  painter.setPen( QColor( 90, 90, 150 ) );
  painter.drawLine( last-1, m_zero, last, m_zero );

  painter.setPen( m_pen );
  painter.drawLine( last-1, m_sampleP, last, m_sample );
  m_sampleP = m_sample;

  painter.end();

  update();
}
 
void PlotterRenderer::setTick( int tickUs )
{
  QString unit = " S";

  double temp = tickUs/1e6;

  if( temp < 1 )
    {
      unit = " mS";
      temp = tickUs/1e3;
      if( temp < 1 )
        {
          unit = " uS";
          temp = tickUs;
        }
    }
  m_tick.setNum(temp);
  m_tick += unit;
}

void PlotterRenderer::paintEvent( QPaintEvent * /* event */ )
{
  QPainter painter( this );

  int origX = width()-m_width;
  painter.drawPixmap( origX, 0, /*m_width, height(),*/ pixmap );
  painter.setPen( QColor( 255, 255, 255 ) );
  painter.drawText( 0, 5, 100, 20, Qt::AlignHCenter, "Tick: "+m_tick );

  painter.end();
}

}
