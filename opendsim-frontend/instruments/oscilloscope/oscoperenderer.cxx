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

#include <QBrush>
#include <QPen>
#include <QFont>
#include <QPainter>

#include "oscoperenderer.h"

namespace dsim
{

OscopeRenderer::OscopeRenderer( int width, int height, QWidget *parent /*= 0l*/ )
            : QWidget( parent )
            , m_samples( 0l )
            , m_sampleSize( 0 )
            , m_width( width )
            , m_height( height )
            , m_hCenter( (double)width/2 )
            , m_vCenter( (double)height/2 )
            , m_scaleX( 0 )
            , m_scaleY( 0 )
            , m_margin( 0 )
{
  setSamples( 0l, 128 );
}

QSize OscopeRenderer::sizeHint() const { return QSize( m_width, m_height ); }

QSize OscopeRenderer::minimumSizeHint() const { return QSize( m_width, m_height );  }

void OscopeRenderer::setSamples( const int samples[], int size )
{
  if( m_sampleSize != size )
    {
      m_sampleSize = size;
      m_scaleX = ((double)m_width)/180;
      m_scaleY = ((double)m_height)/180;
      m_margin = 2 * m_scaleX;
      m_scaleX = ((double)m_width-4*m_scaleX)/m_sampleSize;
      m_scaleY = ((double)m_height-4*m_scaleY)/m_sampleSize;
    }
  m_samples = samples;
  update();
}

void OscopeRenderer::paintEvent( QPaintEvent* /* event */ )
{
  QPainter painter( this );

  painter.setRenderHint( QPainter::Antialiasing, true );

  painter.setBrush( QColor( 5, 9, 29 ) );
  painter.drawRect(0, 0, m_width, m_height );

  double cero = m_margin;
  double endX = m_width - m_margin;
  double endY = m_height - m_margin;
  QPen pen( QColor( 75, 75, 75 ), 1.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin );
  painter.setPen( pen );
  painter.drawLine( cero, m_vCenter, endX, m_vCenter );
  painter.drawLine( m_hCenter, cero, m_hCenter, endY );

  painter.setPen( QColor( 72, 72, 72 ) );

  for( int x = cero; x<endX; x+=20*m_scaleX )
    {
      painter.drawLine( x, cero, x, endY );
      painter.drawLine(-x, cero,-x, endY );
    }
  for( int y = cero; y<endY; y+=20*m_scaleY )
    {
      painter.drawLine( cero, y, endX, y);
      painter.drawLine( cero,-y, endX,-y);
    }

  if( m_samples )
    {
      QPen pen2( QColor( 240, 240, 100 ), 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin );

      painter.setPen( pen2 );

      QPointF lastP = QPointF( m_margin, m_vCenter-(double)m_samples[0]*m_scaleY );
      for( int i=1; i<m_sampleSize; i++ )
        {
          QPointF thisP = QPointF( (double)i*m_scaleX+m_margin, m_vCenter-(double)m_samples[i]*m_scaleY );
          painter.drawLine( lastP, thisP );
          lastP = thisP;
        }
    }

  QPen pen3( QColor( 200, 200, 200 ), 1.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin );
  painter.setPen( pen3 );
}

}
