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
            , m_vMax( 0 )
            , m_vMin( 0 )
            , m_margin( 15*m_scale )
            , m_scale( ((double)width-30*m_scale)/512 )
{
}

QSize OscopeRenderer::sizeHint() const { return QSize( m_width, m_height ); }

QSize OscopeRenderer::minimumSizeHint() const { return QSize( m_width, m_height );  }

void OscopeRenderer::setVsize( double max, double min ) { m_vMax = max; m_vMin = min; }

void OscopeRenderer::setSamples( const int samples[], int size )
{
  m_samples = samples;
  m_sampleSize = size;
  m_scale = ((double)m_width-30*m_scale)/m_sampleSize;
  update();
}

void OscopeRenderer::paintEvent( QPaintEvent* /* event */ )
{
  QPainter p( this );

  p.setRenderHint( QPainter::Antialiasing, true );

  p.setBrush( QColor( 10, 15, 50 ) );
  p.drawRect(0, 0, m_width, m_height );

  QPen pen( QColor( 90, 90, 180 ), 1.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin );
  p.setPen( pen );

  double cero = m_margin;
  double end = m_width-m_margin;
  p.drawLine( cero, m_vCenter, end, m_vCenter );
  p.drawLine( m_hCenter, cero, m_hCenter, end );

  p.setPen( QColor( 70, 70, 140 ) );

  for( double i=cero; i<end+1; i+=20*m_scale )
    {
      p.drawLine( i, cero, i, end );
      p.drawLine( cero, i, end, i );
    }

  if( m_samples )
    {
      QPen pen2( QColor( 240, 240, 100 ), 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin );

      p.setPen( pen2 );

      QPointF lastP = QPointF( m_margin, end-(double)m_samples[0]*m_scale );
      for( int i=1; i<m_sampleSize; i++ )
        {
          QPointF thisP = QPointF( (double)i*m_scale+m_margin, end-(double)m_samples[i]*m_scale );
          p.drawLine( lastP, thisP );
          lastP = thisP;
        }
    }

  QPen pen3( QColor( 200, 200, 200 ), 1.5, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin );
  p.setPen( pen3 );

  QFont font = p.font();
  font.setPointSize(7);
  font.setBold(true);
  p.setFont( font );

  p.drawText( cero, 1,     end, m_margin, Qt::AlignCenter, "Max: "+QString::number(m_vMax,'f', 2)+" V" );
  p.drawText( cero, end+1, end, m_margin, Qt::AlignCenter, "Min: "+QString::number(m_vMin,'f', 2)+" V" );

  p.end();
}

}
