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
#include <frontend/device-lib.h>
#include <dsim/circmatrix.h>
#include <dsim/circuit.h>
#include <model/circ-node.h>
#include <model/circ-pin.h>
#include <model/circ-element.h>
#include <frontend/error.h>

#include <algorithm>
#include <functional>
#include <string>
#include <cstring>
#include <QtConcurrent>
#include <QThread>

#include "schemaview.h"
#include "elementwire.h"
#include "elementjoint.h"
#include "elementpin.h"
#include "elementabstractport.h"
#include "componentgraphitem.h"
#include "schematicimpl.h"
#include "instrumentmanagement.h"
#include "propertycontainerimpl.h"

#include "schemasheet.h"

namespace dsim
{

SchemaSheet::SchemaSheet()
            : m_schemaView( 0l )
            , m_instrumentManagement( new InstrumentManagement )
            , m_matrix( 0l )
            , m_circuit( 0l )
            , m_compiled( false )

{}

SchemaSheet::~SchemaSheet() { uninit(); }

int SchemaSheet::init()
{
  m_compiled = false;
  m_matrix = matrix_create();
  if( m_matrix )
    {
      m_circuit = circuit_create( m_matrix );
      return m_circuit ? 0 : -DS_CREATE_CIRCUIT_SIMULATOR;
    }
  return -DS_CREATE_CIRCUIT_MATRIX;
}

void SchemaSheet::uninit()
{
  if( m_circuit )
    { circuit_free( m_circuit ); m_circuit = 0l; } // will free attached elements as well
  if( m_matrix )
    { matrix_free( m_matrix ); m_matrix = 0l; }
  m_compiled = false;
}

int SchemaSheet::reinit()
{
  uninit();
  return init();
}

int SchemaSheet::createDevice( const char *symbol, const char *reference, int id, SchematicImpl *schematic, PropertyContainerImpl *property, DS_OUT IDevice **ppdevice )
{
  return createDevice( device_lib_entry( symbol ), reference, id, schematic, property, ppdevice );
}

int SchemaSheet::createDevice( const DeviceLibraryEntry *entry, const char *reference, int id, SchematicImpl *schematic, PropertyContainerImpl *property, DS_OUT IDevice **ppdevice )
{
  *ppdevice = 0l;
  if( !entry ) return -DS_FAULT;

  int rc;
  IDevice *device = entry->construct( reference, id, 0l );
  if( device )
    {
      device->setCircuit( m_circuit );
      if( (rc = device->create( schematic, property )) )
        {
          delete device;
          return rc;
        }

      *ppdevice = device;
      return rc;
    }
  return -DS_NO_MEMORY;
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
  m_compiled = false;
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

int SchemaSheet::compile()
{
  if( m_compiled ) return 0;
  int rc = generateNetlist(); UPDATE_RC(rc);
  end();

  /*
   * Connect physical nodes
   */
  circ_node_t *circNode;
  IDevice *device;
  int pinIndex; bool isInt = false;
  foreach( SchemaNode *node, *(nodes()) )
    {
      circNode = circ_node_create( m_circuit, true );
      if( circNode )
        {
          circuit_attach_node( m_circuit, circNode );
          foreach( ElementAbstractPort *port, *(node->ports()) )
            {
              device = port->component()->device()->get();
              pinIndex = port->reference().toInt( &isInt )-1;     // parse target pin
              if( isInt && pinIndex >= 0 && pinIndex < device->pin_count() )
                {
                  rc = circ_pin_set_node( device->pin( pinIndex ), circNode ); UPDATE_RC(rc);
                }
              else
                return -DS_INVALID_PIN_REFERENCE;
            }
        }
      else
        return -DS_CREATE_CIRCUIT_NODE;
    }

  m_components.clear();

  /*
   * Initializing device models
   */
  foreach( SchemaNode *node, *(nodes()) )
    {
      foreach( ElementAbstractPort *port, *(node->ports()) )
        {
          ComponentGraphItem *component = port->component();

          if( 0== m_components[component] )
            {
              m_components[component] = 1;
              rc = component->initDevice(); UPDATE_RC(rc);
            }
        }
    }

  rc = circuit_init( m_circuit ); UPDATE_RC(rc);
  m_compiled = true;
  return 0;
}

int SchemaSheet::runStep()
{ return circuit_run_step( m_circuit ); }

int SchemaSheet::runLoop()
{
  int rc;
  int tick = 0;
  for( ;; )
    {
      if( tick++ > 10000000 )
        {
          tick = 0;
          m_instrumentManagement->clockTick();
          if( (rc = circuit_run_step( m_circuit )) ) return rc;
        }
      if( m_canceled ) break;
    }
  return 0;
}

int SchemaSheet::run()
{
  if( m_stepFuture.isFinished() )
    {
      m_canceled = false;
      m_stepFuture = QtConcurrent::run( this, &SchemaSheet::runLoop );
    }
  return 0;
}

int SchemaSheet::end()
{
  m_canceled = true;
  m_stepFuture.waitForFinished();

  int rc = reinit(); if( rc ) return -DS_RECREATE_CIRCUIT;

  m_components.clear();

  foreach( SchemaNode *node, *(nodes()) )
    {
      foreach( ElementAbstractPort *port, *(node->ports()) )
        {
          ComponentGraphItem *component = port->component();
          if( 0== m_components[component] )
            {
              m_components[component] = 1;
              rc = component->createDevice(); if( rc ) return -DS_RECREATE_CIRCUIT; // create the device again
            }
        }
    }

  return rc;
}

bool SchemaSheet::running()
{ return m_stepFuture.isRunning() && m_canceled; }

void SchemaSheet::setSchemaView( SchemaView *schemaView )
{ m_schemaView = schemaView; }

}
