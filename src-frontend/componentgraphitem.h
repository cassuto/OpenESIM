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

#ifndef COMPONENTGRAPHITEM_H_
#define COMPONENTGRAPHITEM_H_

#include <QList>
#include <QtWidgets>
#include "componentgraph.h"

#include "elementgraphitem.h"

class IDevice;
class IDeviceGraph;

namespace dsim
{

class SchemaGraph;

class ComponentGraphItem : public ElementGraphItem
{
public:
  ComponentGraphItem( int id, bool edit, IDevice *device, SchemaGraph *schgraph, QGraphicsItem *parent = 0 );
  ~ComponentGraphItem();

  const char *classname() { return "component"; }

public:
  inline IDevice *device() const { return m_device; }
  inline IDeviceGraph *deviceGraph() const { return m_deviceGraph; }

protected:
  void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget );

private:
  IDevice           *m_device;
  ComponentGraph    *m_deviceGraph;
  SchemaGraph       *m_schgraph;
};

}

#endif //!defined(COMPONENT_H_)
