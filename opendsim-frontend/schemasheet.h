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

#ifndef SCHEMASHEET_H_
#define SCHEMASHEET_H_

#include <QList>
#include <QHash>
#include <QFuture>
#include <dsim/circmatrix.h>
#include <dsim/circuit.h>

#include "schematicimpl.h"

class IDevice;
class ISchematic;
class DeviceLibraryEntry;

namespace dsim
{

class SchemaView;
class ComponentGraphItem;
class SchematicImpl;
class PropertyContainerImpl;
class ElementAbstractPort;
class InstrumentManagement;

class SchemaSheet
{
public:
  SchemaSheet();
  ~SchemaSheet();

  class SchemaNode
  {
  public:
    void addPort( ElementAbstractPort *port ) { m_ports.append( port ); }
    inline const QList<ElementAbstractPort *> *ports() const { return &m_ports; }
  public:
    QList<ElementAbstractPort *> m_ports;
  };

  int init();
  void uninit();
  int reinit();
  int createDevice( const char *symbol, const char *reference, int id, SchematicImpl *schematic, PropertyContainerImpl *property, DS_OUT IDevice **ppdevice );
  int createDevice( const DeviceLibraryEntry *entry, const char *reference, int id, SchematicImpl *schematic, PropertyContainerImpl *property, DS_OUT IDevice **ppdevice );

  int generateNetlist();
  inline const QList<SchemaNode *> *nodes() const { return &m_nodes; }
  int compile();
  int runStep();
  int run();
  int end();
  bool running();

  void setSchemaView( SchemaView *schemaView );
  inline SchemaView *schemaView() const { return m_schemaView; }
  inline InstrumentManagement *instrumentManagement() const { return m_instrumentManagement; }
private:
  int runLoop();

private:
  SchemaView        *m_schemaView;
  InstrumentManagement *m_instrumentManagement;
  circ_matrix_t     *m_matrix;
  circuit_t         *m_circuit;
  QList<SchemaNode *> m_nodes;
  QHash<ComponentGraphItem *, char> m_components;
  bool               m_compiled;
  bool volatile      m_canceled;
  QFuture<int>       m_stepFuture;
};

}

#endif //!defined(SCHEMASHEET_H_)
