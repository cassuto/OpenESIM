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

#ifndef SCHEMATICIMPL_H_
#define SCHEMATICIMPL_H_

#include <device/schematic.h>
#include <device/device.h>
class IDeviceGraph;

namespace dsim
{

class SchemaView;
class ComponentGraphItem;
class ComponentGraphImpl;

class SchematicImpl : public ISchematic
{
public:
  SchematicImpl( SchemaView *schemaView, ComponentGraphItem *component );

public:
  void reset();
  void emitEvent( render_event_t event, int x, int y, long flags );

public:
  void changeValue( const char *value );
  IDeviceGraph *getDeviceGraph( const char *tokenId );
  void registerRender( IDevice *device );
  void acceptRenderEvent( bool accept );
  void update();

private:
  SchemaView *m_schemaView;
  ComponentGraphItem *m_component;
  bool          m_acceptRenderEvent;
};

}

#endif
