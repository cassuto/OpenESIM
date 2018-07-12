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
# include "templatestyle.h"
#endif

#ifndef TEMPLATESTYLEFUNCTIONS_H_
#define TEMPLATESTYLEFUNCTIONS_H_

#include <QBrush>
#include <QPen>
#include <QFont>
#include <QGraphicsSimpleTextItem>

namespace dsim
{

template <class T>
  void Templatestyle::apply( T *painter, const char *style, bool selected )
  {
    if( !Templatestyle::instance()->isStyle( style ) )
      return;

    StyleItem s = Templatestyle::instance()->fillStyle( style, selected );

    QBrush brush = painter->brush();
    if( s.usebkcolor )
      {
        brush.setStyle( Qt::NoBrush );
      }
    else
      {
        brush.setColor( QColor( s.color.r, s.color.g, s.color.b ) );
      }

    s = Templatestyle::instance()->lineStyle( style, selected );

    QPen pen = painter->pen();
    pen.setStyle( Templatestyle::toQtPenStyle( s.line ) );
    if( s.line != LINE_NONE)
      {
        pen.setWidthF( s.width );
        pen.setColor( QColor( s.color.r, s.color.g, s.color.b ) );
      }

    painter->setBrush( brush );
    painter->setPen( pen );
  }

template <>
  inline void Templatestyle::apply( QGraphicsSimpleTextItem *text, const char *style, bool selected )
  {
    if( !Templatestyle::instance()->isStyle( style ) )
        return;

    StyleItem s = Templatestyle::instance()->textStyle( style, selected );

    QFont font = text->font();
    font.setPointSize( s.size );
    font.setBold( s.bold );
    font.setItalic( s.italic );
    text->setFont( font );

    QBrush brush = text->brush();
    brush.setColor( QColor( s.color.r, s.color.g, s.color.b ) );
    text->setBrush( brush );
  }

}

#endif
