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

#ifndef TEMPLATESTYLE_H_
#define TEMPLATESTYLE_H_

#include <QColor>

#include <device/graph.h>
#include <dsim/types.h>

class QGraphicsSimpleTextItem;

namespace dsim
{

class StyleItem
{
public:
  StyleItem() : line( LINE_SOLID ), brush( BRUSH_NONE ), width( 1.0 ), weight( 0 ), bold( false )
              , italic( false )
    {
      color.r = color.g = color.b = -1;
      usebkcolor = true;
    }
public:
  LineStyle     line;
  BrushStyle    brush;
  float         width;
  ds_color_t    color;
  bool          usebkcolor;
  int           weight;
  bool          bold;
  bool          italic;
};

class Templatestyle
{
public:
  Templatestyle();
  ~Templatestyle();

  static Templatestyle *instance() { return m_ptemplatestyle; }

  bool isStyle( const char *style );

  StyleItem lineStyle( const char *style, bool selected );
  StyleItem fillStyle( const char *style, bool selected );
  StyleItem textStyle( const char *style, bool selected );

  static Qt::PenStyle toQtPenStyle( LineStyle line );
  static Qt::BrushStyle toQtBrushStyle( BrushStyle brush );

  void applyStyle( QGraphicsSimpleTextItem *text, const char *style, bool selected );

  template<class T>
    static void applyStyle( T *painter, const char *style, bool selected );

private:
  static Templatestyle *m_ptemplatestyle;
};

}

#include "templatestylefunctions.hxx"

#endif
