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
#include <string>
#include "componentgraph.h"

#include "elementgraphitem.h"

class IDevice;
class IDeviceGraph;
class DeviceLibraryEntry;

namespace dsim
{

class ElementText;

class ComponentGraphItem : public ElementGraphItem<QGraphicsItem>
{
public:
  ComponentGraphItem( int id, SchemaGraph *scene, bool edit, QGraphicsItem *parent = 0 );
  ~ComponentGraphItem();

  const char *classname() { return "component"; }
  QRectF boundingRect() const { return m_bounding; }
  void setBoundingRect( const QRect &bounding );

  int init( const char *deviceEntry, ElementText *symbolText, ElementText *referenceText, bool deser = false );
  void setLayout();
  std::string reference();

  int serialize( LispDataset *dataset );
  int deserialize( LispDataset *dataset );
  int resolveSubElements();

public:
  inline IDevice *device() const { return m_device; }
  inline IDeviceGraph *deviceGraph() const { return m_deviceGraph; }

protected:
  void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget );

private:
  void setSubs( ElementText *symbolText, ElementText *referenceText );

private:
  QRectF                m_bounding;
  IDevice              *m_device;
  ComponentGraph       *m_deviceGraph;
  std::string           m_symbol;
  std::string           m_reference; // fpr deserialize
  ElementText          *m_symbolText;
  ElementText          *m_referenceText;
};

}

#endif //!defined(COMPONENT_H_)
