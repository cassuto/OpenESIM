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

#ifndef PLOTTERRENDERER_H_
#define PLOTTERRENDERER_H_

#include <QBrush>
#include <QPen>
#include <QPixmap>
#include <QtWidgets>

namespace dsim
{

class PlotterRenderer : public QWidget
{
public:
  PlotterRenderer( int width, int height, QWidget *parent = 0l );

  QSize minimumSizeHint() const;
  QSize sizeHint() const;

  void setSample( int sample );
  void printSample();
  void printGrid();

  void setTick( int tickUs );
  void setZero( int zero );
  void setPen( const QPen &pen );
  void setBrush( const QBrush &brush );
  void setBkcolor( const QColor &color );
  void setAntialiased( const bool antialiased );

protected:
  void paintEvent( QPaintEvent *event );

private:
  QPen m_pen;
  QBrush brush;
  QColor m_bkcolor;
  bool antialiased;
  QPixmap pixmap;

  int m_sample;
  int m_sampleP;
  int m_width;
  int m_height;
  int m_sec;
  int m_zero;

  QString m_tick;
};

}

#endif
