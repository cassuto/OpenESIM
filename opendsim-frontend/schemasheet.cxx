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

#define TRACE_UNIT "schemasheet"

#include <dsim/logtrace.h>
#include <dsim/device-lib.h>
#include <dsim/circmatrix.h>
#include <dsim/circuit.h>
#include <dsim/error.h>

#include <algorithm>
#include <functional>
#include <string>
#include <cstring>
#include <cstdio>

#include "schemaview.h"
#include "elementwire.h"
#include "elementjoint.h"
#include "elementpin.h"
#include "componentgraphitem.h"
#include "schematicimpl.h"
#include "schemasheet.h"

namespace dsim
{

SchemaSheet::SchemaSheet()
            : m_schemaView( 0l )
{
  m_schematic = new SchematicImpl;

  m_matrix = matrix_create();
  m_circuit = circuit_create( m_matrix );
}

SchemaSheet::~SchemaSheet()
{
  circuit_free( m_circuit );
  matrix_free( m_matrix );
}

int SchemaSheet::createDevice( const char *symbol, const char *reference, int id, DS_OUT IDevice **ppdevice )
{
  return createDevice( device_lib_entry( symbol ), reference, id, ppdevice );
}

int SchemaSheet::createDevice( const DeviceLibraryEntry *entry, const char *reference, int id, DS_OUT IDevice **ppdevice )
{
  *ppdevice = 0l;
  if( !entry ) return -DS_FAULT;

  IDevice *device = entry->construct( reference, id, m_circuit, 0l );
  if( device )
    {
      int rc = device->create( schematic() );
      if( rc )  // failed to create the device
        {
          delete device;
          return rc;
        }
      *ppdevice = device;
      return rc;
    }
  return -DS_NO_MEMORY;
}

void SchemaSheet::deleteDevice( IDevice *device )
{
  delete device;
}

void SchemaSheet::addDevice( IDevice *device )
{
}

void SchemaSheet::removeDevice( IDevice *device )
{
}


static inline int setsFind( int *sets, int x )
{
  while( x != sets[x])
    {
      sets[x] = sets[sets[x]]; // path compression
      x = sets[x];
    }
  return x;
}

static inline void unionSets( int *sets, int *next, int *last, int x, int y )
{
  int fx=setsFind( sets, x) , fy=setsFind( sets, y );
  if( fx==fy ) return;

  sets[fx] = fy;
  next[last[fy]] = fx;
  last[fy] = last[fx];
}

int SchemaSheet::generateNetlist() // m_nodes is valid while schemaView() keeping not updated.
{
  int setsSize = 0;
  QList<ElementAbstractPort *> ports;
  QHash<ElementAbstractPort *, char> portVisited; // store the index of each port, beginning with # 1

  foreach( ElementBase *element, schemaView()->elements() ) // extract ports
    {
      if( 0==std::strcmp( element->classname(), "wire" ) )
        {
          ElementWire *wire = static_cast<ElementWire *>(element);

          if( 0 == portVisited[wire->startPort()] )
            {
              ports.append( wire->startPort() );
              portVisited[wire->startPort()] = ports.size();
            }
          if( 0 == portVisited[wire->endPort()] )
            {
              ports.append( wire->endPort() );
              portVisited[wire->endPort()] = ports.size();
            }
        }
      else if ( 0==std::strcmp( element->classname(), "joint" ) )
        {
          ElementJoint *joint = static_cast<ElementJoint *>(element);

          for( int x=0; x<3; x++ )
            {
              if( 0 == portVisited[joint->port( x )] )
                {
                  ports.append( joint->port( x ) );
                  portVisited[joint->port( x )] = ports.size();
                }
            }
        }
    }

  if( !(setsSize = ports.size()) ) return 0;
  ++setsSize;

  int *sets = new int[ setsSize ];
  int *next = new int[ setsSize ];
  int *last = new int[ setsSize ];
  for( int i=1; i<setsSize; i++ )
    {
      sets[i] = last[i] = i; // reset roots
      next[i] = -1;
    }

  foreach( ElementBase *element, schemaView()->elements() ) // merge connections
    {
      if( 0==std::strcmp( element->classname(), "wire" ) )
        {
          ElementWire *wire = static_cast<ElementWire *>(element);

          int s = portVisited[wire->startPort()];
          int e = portVisited[wire->endPort()];

          unionSets( sets, next, last, e, s );
        }
      else if ( 0==std::strcmp( element->classname(), "joint" ) )
        {
          ElementJoint *joint = static_cast<ElementJoint *>(element);

          int idx[3];
          for( int x=0; x<3; x++ )
            {
              idx[x] = portVisited[joint->port( x )];
            }
          unionSets( sets, next, last, idx[0], idx[1] );
          unionSets( sets, next, last, idx[0], idx[2] );
          unionSets( sets, next, last, idx[1], idx[2] );
        }
    }

  m_nodes.clear();

  for( int x=1; x<setsSize; x++ )
    {
      if( setsFind( sets, x )==x ) // find the root node
        {
          SchemaNode *node = 0l;
          for( int i=x; i > 0; i = next[i] )
            {
              if( ports.at(i-1)->component() )
                {
                  if( UNLIKELY(!node) ) node = new SchemaNode;
                  node->addPort( ports.at(i-1) );
                }
            }
          if( node ) m_nodes.append( node );
        }
    }

  return 0;
}

void SchemaSheet::setSchemaView( SchemaView *schemaView )
{
  m_schematic->setSchemaView( schemaView );
  m_schemaView = schemaView;
}

}
