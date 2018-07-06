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

#include <cstring>

#include "templatestyle.h"

namespace dsim
{

TemplateStyle::TemplateStyle()
{
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

  if( 0 == strcmp( styleName, "symbol" ) )
    {
      if( selected )
        { style.color.r = 128; style.color.g = 0; style.color.b = 0; }
      else
        { style.color.r = 0; style.color.g = 0; style.color.b = 128; }
    }
  return style;
}

}
