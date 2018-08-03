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

#ifndef PROPERTYWIDGETPIN_H_
#define PROPERTYWIDGETPIN_H_

#include "propertywidget.h"
#include <device/device.h> // for io_type_t

namespace dsim
{

class ElementPin;
class PropertyWidgetPin : public PropertyWidget
{
  Q_OBJECT

public:
  PropertyWidgetPin( ElementPin *pin, QWidget *parent = 0l );

  const char *name() const { return "Pin"; }
  void accept();

private:
  const char *ioType2String( io_type_t io );
  io_type_t string2IoType( const char *string );

private:
  QLineEdit *m_symbolEdit;
  QLineEdit *m_referenceEdit;
  QSpinBox *m_lengthSpin;
  QCheckBox *m_showSymbol;
  QCheckBox *m_showReference;
  QComboBox *m_ioType;
};

}

#endif
