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

#ifndef TEMPLATECUSTOM_H_
#define TEMPLATECUSTOM_H_

#include "domitem.h"
#include <device/graph.h>

namespace dsim
{

class StyleItem;

class TemplateCustom: public DomItem
{
public:
  TemplateCustom();

  int serialize( LispDataset *dataset );
  int deserialize( LispDataset *dataset );

  void setCustomAll( bool custom );
  bool custom() const;
  void apply( StyleItem *style ) const;

public:
  LineStyle     line;
  bool          lineCustom;
  BrushStyle    brush;
  bool          brushCustom;
  float         width;
  bool          widthCustom;
  ds_color_t    color;
  bool          colorCustom;
  bool          usebkcolor;
  bool          usebkcolorCustom;
  ds_color_t    bkcolor;
  bool          bkcolorCustom;
  int           size; // 1 points = 1/72 inch
  bool          sizeCustom;
  bool          bold;
  bool          boldCustom;
  bool          italic;
  bool          italicCustom;
};

}

#endif

