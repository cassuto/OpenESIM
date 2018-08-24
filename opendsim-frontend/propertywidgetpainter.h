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

#ifndef PROPERTYWIDGETPAINTER_H_
#define PROPERTYWIDGETPAINTER_H_

#include "propertywidget.h"

namespace dsim
{

class ElementPainter;
class PropertyWidgetPainter : public PropertyWidget
{
  Q_OBJECT
public:
  PropertyWidgetPainter( ElementPainter *painter, QWidget *parent = 0l );
  ~PropertyWidgetPainter();

  const char *name() const { return "Painter"; }
  void firstPage();
  void accept();

private:
  QComboBox *m_tokenId;
};

}

#endif
