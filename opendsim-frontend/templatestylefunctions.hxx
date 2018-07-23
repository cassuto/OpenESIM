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

#include "templatecustom.h"

namespace dsim
{

template <class T>
  void Templatestyle::apply( T *painter, TemplateCustom *customLine, TemplateCustom *customFill, const char *style, bool selected )
  {
    if( !Templatestyle::instance()->isStyle( style ) )
      return;

    StyleItem s = Templatestyle::instance()->fillStyle( style, selected );
    if( customFill ) customFill->apply( &s );

    QBrush brush = painter->brush();
    if( s.usebkcolor )
      {
        brush.setStyle( Qt::NoBrush );
      }
    else
      {
        if( s.bkcolor.r >= 0 && s.bkcolor.g >= 0 && s.bkcolor.b >= 0 )
          brush.setColor( QColor( s.bkcolor.r, s.bkcolor.g, s.bkcolor.b ) );
      }

    s = Templatestyle::instance()->lineStyle( style, selected );
    if( customLine ) customLine->apply( &s );

    QPen pen = painter->pen();
    pen.setStyle( Templatestyle::toQtPenStyle( s.line ) );
    if( s.line != LINE_NONE)
      {
        pen.setWidthF( s.width );
        if( s.color.r >= 0 && s.color.g >= 0 && s.color.b >= 0 )
          {
            pen.setColor( QColor( s.color.r, s.color.g, s.color.b ) );
          }
      }

    painter->setBrush( brush );
    painter->setPen( pen );
  }

}

#endif
