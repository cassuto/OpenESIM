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

#define TRACE_UNIT "temp"

#include <dsim/logtrace.h>
#include <cstring>

#include "templatestyle.h"

namespace dsim
{

TemplateStyle *TemplateStyle::m_ptemplatestyle = 0l;

TemplateStyle::TemplateStyle()
{
  trace_assert( 0l==m_ptemplatestyle );
  m_ptemplatestyle = this;
}
TemplateStyle::~TemplateStyle()
{
  m_ptemplatestyle = 0l;
}

bool TemplateStyle::isStyle( const char *style )
{
  if( 0 == strcmp( style, "component" ) )
    return true;
  else if( 0 == strcmp( style, "pin" ) )
    return true;
  return false;
}

StyleItem TemplateStyle::lineStyle( const char *styleName, bool selected )
{
  StyleItem style;

  if( 0 == strcmp( styleName, "component" ) )
    {
      style.line = LINE_SOLID;
      style.width = 1.0f;
      if( selected )
        { style.color.r = 128; style.color.g = 0; style.color.b = 0; }
      else
        { style.color.r = 0; style.color.g = 0; style.color.b = 128; }
    }
  else if( 0 == strcmp( styleName, "pin" ) )
    {
      style.line = LINE_SOLID;
      style.width = 1.0f;
      if( selected )
        { style.color.r = 128; style.color.g = 0; style.color.b = 0; }
      else
        { style.color.r = 0; style.color.g = 0; style.color.b = 0; }
    }
  return style;
}

StyleItem TemplateStyle::fillStyle( const char *styleName, bool selected )
{
  StyleItem style;

  if( 0 == strcmp( styleName, "component" ) )
    {
      style.line = LINE_SOLID;
      style.usebkcolor = true;
    }
  return style;
}

StyleItem TemplateStyle::textStyle( const char *styleName, bool selected )
{
  StyleItem style;

  if( 0 == strcmp( styleName, "component" ) )
    {
      if( selected )
        { style.color.r = 128; style.color.g = 0; style.color.b = 0; }
      else
        { style.color.r = 0; style.color.g = 0; style.color.b = 128; }
      style.weight = 12;
    }
  else if( 0 == strcmp( styleName, "pin" ) )
    {
      if( selected )
        { style.color.r = 128; style.color.g = 0; style.color.b = 0; }
      else
        { style.color.r = 128; style.color.g = 0; style.color.b = 0; }
      style.weight = 12;
    }
  return style;
}

Qt::PenStyle TemplateStyle::toQtPenStyle( LineStyle line )
{
  switch( line )
  {
    case LINE_NONE:         return Qt::NoPen;
    case LINE_SOLID:        return Qt::SolidLine;
    case LINE_DASH:         return Qt::DashLine;
    case LINE_DOT:          return Qt::DotLine;
    case LINE_DASH_DOT:     return Qt::DashDotLine;
    case LINE_DASH_DOTDOT:  return Qt::DashDotDotLine;
    default:                return Qt::NoPen;
  }
}

Qt::BrushStyle TemplateStyle::toQtBrushStyle( BrushStyle brush )
{
  switch( brush )
  {
    case BRUSH_NONE:        return Qt::NoBrush;
    case BRUSH_SOLID:       return Qt::SolidPattern;
    case BRUSH_HOR:         return Qt::HorPattern;
    case BRUSH_VER:         return Qt::VerPattern;
    case BRUSH_CROSS:       return Qt::CrossPattern;
    default:                return Qt::NoBrush;
  }
}

}
