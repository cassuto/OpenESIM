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
#include "componentgraphimpl.h"
#include "transformations.h"

#include <instrument/pointerproxy.h>
#include "elementgraphitem.h"

class IDevice;
class IDeviceGraph;
class DeviceLibraryEntry;

namespace dsim
{

class PropertyContainerImpl;
class SchematicImpl;
class ComponentGraphImpl;
class ElementText;
class ElementPin;

class ComponentGraphItem : public ElementGraphItem<QGraphicsItemGroup>
{
public:
  ComponentGraphItem( int id, SchemaScene *scene, bool edit, QGraphicsItem *parent = 0l );
  ~ComponentGraphItem();

  const char *classname() const { return "component"; }

  int init( const char *deviceEntry, ElementText *valueText, ElementText *referenceText );
  int createDevice();
  int initDevice();
  void resetId( int id );

  void setLayout();
  std::string reference();
  std::string value();
  std::string symbol();
  void setReference( const QString &reference );
  void setValue( const QString &value );
  void syncParameters();
  void setDirect( ElemDirect direct );
  int addComponentElement( ElementBase *element );
  ElementPin *atPin( const QPointF &pos );
  QList<ElementBase *>::iterator pinsBegin();
  QList<ElementBase *>::iterator pinsEnd();

  int serialize( LispDataset *dataset );
  int deserialize( LispDataset *dataset );
  int resolveSubElements();
  void releaseSubElements();

public:
  inline PointerProxy<IDevice> *device() const { return m_device; }
  inline ComponentGraphImpl *deviceGraph() const { return m_deviceGraph; }
  inline PropertyContainerImpl *properties() const { return m_properties; }
  inline SchematicImpl *schematic() const { return m_schematic; }

protected:
  QVariant itemChange( GraphicsItemChange change, const QVariant &value );
  void mouseReleaseEvent( QGraphicsSceneMouseEvent *event );
  void paint( QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget );

private:
  int findEntry( const char *symbol );
  void addComponentElementInner( ElementBase *element );

private:
  const DeviceLibraryEntry *m_deviceEntry;
  PointerProxy<IDevice> *m_device;
  std::string           m_symbol; // guarantee synchronization among serialize() deserialize() and init()
  std::string           m_reference; // guarantee synchronization among serialize() deserialize() and init()
  std::string           m_value; // guarantee synchronization among serialize() deserialize() and init()
  ElementText          *m_valueText;
  ElementText          *m_referenceText;
  SchematicImpl        *m_schematic;
  PropertyContainerImpl *m_properties;
  ComponentGraphImpl   *m_deviceGraph;
};

}

#endif //!defined(COMPONENT_H_)
