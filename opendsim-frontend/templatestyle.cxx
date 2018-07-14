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

Templatestyle *Templatestyle::m_ptemplatestyle = 0l;

Templatestyle::Templatestyle()
{
  trace_assert( 0l==m_ptemplatestyle );
  m_ptemplatestyle = this;
}
Templatestyle::~Templatestyle()
{
  m_ptemplatestyle = 0l;
}

bool Templatestyle::isStyle( const char *style )
{
  if( 0 == strcmp( style, "component" ) )
    return true;
  else if( 0 == strcmp( style, "pin" ) )
    return true;
  else if( 0 == strcmp( style, "wire" ) )
    return true;
  return false;
}

StyleItem Templatestyle::lineStyle( const char *styleName, bool selected )
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
        { style.color.r = 0; style.color.g = 128; style.color.b = 255; }
    }
  else if( 0 == strcmp( styleName, "wire" ) )
    {
      style.line = LINE_SOLID;
      style.width = 1.0f;
      if( selected )
        { style.color.r = 128; style.color.g = 0; style.color.b = 0; }
      else
        { style.color.r = 128; style.color.g = 0; style.color.b = 128; }
    }
  return style;
}

StyleItem Templatestyle::fillStyle( const char *styleName, bool selected )
{
  StyleItem style;

  if( 0 == strcmp( styleName, "component" ) )
    {
      style.line = LINE_SOLID;
      style.usebkcolor = true;
    }
  return style;
}

StyleItem Templatestyle::textStyle( const char *styleName, bool selected )
{
  StyleItem style;

  if( 0 == strcmp( styleName, "component" ) )
    {
      if( selected )
        { style.color.r = 128; style.color.g = 0; style.color.b = 0; }
      else
        { style.color.r = 128; style.color.g = 0; style.color.b = 128; }
      style.size = 8;
    }
  else if( 0 == strcmp( styleName, "pin" ) )
    {
      if( selected )
        { style.color.r = 128; style.color.g = 0; style.color.b = 0; }
      else
        { style.color.r = 0; style.color.g = 0; style.color.b = 255; }
      style.size = 8;
    }
  return style;
}

std::list<std::string> Templatestyle::templates()
{ using namespace std;

  list<string> ret;

  ret.push_front( string("wire") );
  ret.push_front( string("pin") );
  ret.push_front( string("component") );

  return ret;
}

void Templatestyle::apply( QGraphicsSimpleTextItem *text, TemplateCustom *customText, const char *style, bool selected )
{
  if( !Templatestyle::instance()->isStyle( style ) )
      return;

  StyleItem s = Templatestyle::instance()->textStyle( style, selected );
  if( customText ) customText->apply( &s );

  QFont font = text->font();
  font.setPointSize( s.size );
  font.setBold( s.bold );
  font.setItalic( s.italic );
  text->setFont( font );

  QBrush brush = text->brush();
  if( s.color.r >= 0 && s.color.g >= 0 && s.color.b >= 0 )
    {
      brush.setColor( QColor( s.color.r, s.color.g, s.color.b ) );
    }
  text->setBrush( brush );
}

const char *Templatestyle::toLineStyleName( LineStyle line )
{
  switch( line )
  {
    case LINE_NONE:         return "None";
    case LINE_SOLID:        return "Solid";
    case LINE_DASH:         return "Dash";
    case LINE_DOT:          return "Dot";
    case LINE_DASH_DOT:     return "DashDot";
    case LINE_DASH_DOTDOT:  return "DashDotDot";
    default:                return "None";
  }
}

const char *Templatestyle::toBrushStyleName( BrushStyle brush )
{
  switch( brush )
  {
    case BRUSH_NONE:        return "None";
    case BRUSH_SOLID:       return "Solid";
    case BRUSH_HOR:         return "Horizontal Lines";
    case BRUSH_VER:         return "Vertical Lines";
    case BRUSH_CROSS:       return "Cross Lines";
    default:                return "None";
  }
}

Qt::PenStyle Templatestyle::toQtPenStyle( LineStyle line )
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

Qt::BrushStyle Templatestyle::toQtBrushStyle( BrushStyle brush )
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
