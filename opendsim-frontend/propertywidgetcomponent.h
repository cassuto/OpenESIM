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

#ifndef PROPERTYWIDGETCOMPONENT_H_
#define PROPERTYWIDGETCOMPONENT_H_

#include "propertywidget.h"

namespace dsim
{

class ComponentGraphItem;
class PropertyWidgetComponent : public PropertyWidget
{
  Q_OBJECT
public:
  PropertyWidgetComponent( ComponentGraphItem *component, QWidget *parent = 0l );
  ~PropertyWidgetComponent();

  const char *name() const { return "Text"; }
  void firstPage();
  void accept();

private slots:
  void valueTextChanged( const QString &text );

private:
  QComboBox *m_reference;
  QComboBox *m_value;
  QLabel    *m_validValue;
};

}

#endif
