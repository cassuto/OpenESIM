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

#ifndef PROPERTYWIDGET_H_
#define PROPERTYWIDGET_H_

#include <QtWidgets>

namespace dsim
{

class PropertyWidget : public QWidget
{
public:
  PropertyWidget( void *opaque, QWidget *parent = 0l );

  virtual const char *name() const = 0;

  virtual void firstPage() {}  // called when this widget is considered as the first page
  virtual void accept() {}      // called when changes are accepted
  virtual void reject() {}      // called when changes are rejected

  template <class T>
    inline T opaque() { return static_cast<T>(m_opaque); }

private:
  void *m_opaque;
};

}

#endif
