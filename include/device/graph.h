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

#ifndef CIRC_GRAPH_H_
#define CIRC_GRAPH_H_

#include <dsim/types.h>

#if !defined(__cplusplus)
#error cplusplus only
#endif

enum LineStyle
{
  LINE_NONE = 0,
  LINE_SOLID,
  LINE_DASH,
  LINE_DOT,
  LINE_DASH_DOT,
  LINE_DASH_DOTDOT,
  LINE_RESERVED_MAX
};

enum BrushStyle
{
  BRUSH_NONE = 0,
  BRUSH_SOLID,
  BRUSH_HOR,
  BRUSH_VER,
  BRUSH_CROSS,
  BRUSH_RESERVED_MAX
};

enum ElemDirect
{
  ELEM_LEFT = 0,
  ELEM_TOP,
  ELEM_RIGHT,
  ELEM_BOTTOM
};

enum BitmapFormat
{
  BITMAP_FORMAT_INVALID = 0,
  BITMAP_FORMAT_DEFAULT,
  BITMAP_FORMAT_MONO,
  BITMAP_FORMAT_MONOLSB,
  BITMAP_FORMAT_RGB32,
  BITMAP_FORMAT_ARGB32,
  BITMAP_FORMAT_RGB16,
  BITMAP_FORMAT_RGB666,
  BITMAP_FORMAT_RGB555,
  BITMAP_FORMAT_RGB888
};

struct IRECT
{
public:
  int x, y, w, h;

public:
  IRECT(int left, int top, int width, int height)
    : x(left), y(top), w(width), h(height)
  {}

};

class IDeviceGraph
{
public:
  IDeviceGraph() {}
  virtual ~IDeviceGraph() {}

  virtual void setStyle( const char *style )=0;
  virtual void setBrushColor( int r, int g, int b )=0;
  virtual void setBrushStyle( BrushStyle style )=0;
  virtual void setPenColor( int r, int g, int b )=0;
  virtual void setPenWidth( float w )=0;
  virtual void setPenStyle( LineStyle style )=0;
  virtual void begin( BitmapFormat format = BITMAP_FORMAT_DEFAULT )=0;
  virtual void point( int x, int y )=0;
  virtual void line( int x1, int y1, int x2, int y2 )=0;
  virtual void rect( int x, int y, int w, int h )=0;
  //virtual void rectD( int x, int y, int w, int h )=0;
  virtual void roundRect( int x, int y, int w, int h, int xRnd, int yRnd )=0;
  virtual void ellipse( int x, int y, int w, int h )=0;
  virtual void arc( int x, int y, int w, int h, int a, int alen )=0;
  //virtual int  text( const char*, int, int, int *Height=0 )=0;
  //virtual int  textMapped( const char*, int, int, int *Height=0 )=0;
  //virtual void fillRect( int, int, int, int, const graph_color& )=0;
  //virtual void eraseRect( int, int, int, int )=0;
  virtual void fill( int r, int g, int b )=0;
  virtual void setColor( int index, int r, int g, int b )=0;
  virtual void setPixel( int col, int row, unsigned int color )=0;
  virtual void end()=0;
};

#endif //!defined(CIRC_GRAPH_H_)
