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

#ifndef COMPONENTGRAPHIMPL_H_
#define COMPONENTGRAPHIMPL_H_

#include <device/graph.h>

class QPixmap;
class QPainter;

namespace dsim
{

class ElementPainter;

class ComponentGraphImpl : public IDeviceGraph
{
public:
  ComponentGraphImpl();
  ~ComponentGraphImpl();

  inline void setSize( int width, int height ) { m_width = width; m_height = height; }
  void init( ElementPainter *elementPainter );
  void setElement( ElementPainter *elementPainter );
  void setSelected( bool selected ) { m_selected = selected; }

public:
  void setStyle( const char *style );
  void setBrushColor( int r, int g, int b );
  void setBrushStyle( BrushStyle style );
  void setPenColor( int r, int g, int b );
  void setPenWidth( float w );
  void setPenStyle( LineStyle style );
  void begin( BitmapFormat format = BITMAP_FORMAT_DEFAULT );
  void point( int x, int y );
  void line( int x1, int y1, int x2, int y2 );
  void rect( int x, int y, int w, int h );
  void rectD( int x, int y, int w, int h );
  void roundRect( int x, int y, int w, int h, int xRnd, int yRnd );
  void ellipse( int x, int y, int w, int h );
  void arc( int x, int y, int w, int h, int a, int alen );
  int  text( const char*, int, int, int *Height=0 );
  int  textMapped( const char*, int, int, int *Height=0 );
  void fillRect( int, int, int, int, const ds_color_t& );
  void eraseRect( int, int, int, int );
  void fill( int r, int g, int b );
  void setColor( int index, int r, int g, int b );
  void setPixel( int col, int row, unsigned int color );
  void end();

private:
  void createPixBuffer( BitmapFormat format );

private:
  QImage  *m_pixBuffer;
  QPainter *m_painter;
  ElementPainter *m_elementPainter;
  bool      m_selected;
  int       m_width, m_height;
  BitmapFormat m_format;
};

}

#endif
