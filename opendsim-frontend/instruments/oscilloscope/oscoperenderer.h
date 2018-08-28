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

#ifndef OSCOPERENDERER_H_
#define OSCOPERENDERER_H_

#include <QWidget>

namespace dsim
{
  
class OscopeRenderer : public QWidget
{
public:
  OscopeRenderer( int width, int height, QWidget *parent = 0l );

  void setSamples( const int samples[], int size );

public:
  QSize sizeHint() const;
  QSize minimumSizeHint() const;

protected:
  void paintEvent( QPaintEvent *event );

private:
  const int *m_samples;
  int m_sampleSize;

  int m_width;
  int m_height;
  double m_hCenter;
  double m_vCenter;
  double m_scaleX;
  double m_scaleY;
  double m_margin;
};
  
}

#endif
