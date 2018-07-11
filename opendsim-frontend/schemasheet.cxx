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

#include <algorithm>
#include <functional>

#include "schemasheet.h"

namespace dsim
{

SchemaSheet::SchemaSheet()
{
  m_matrix = matrix_create();
  m_circuit = circuit_create( m_matrix );
}

SchemaSheet::~SchemaSheet()
{
  circuit_free( m_circuit );
  matrix_free( m_matrix );
}

IDevice *SchemaSheet::createDevice( const char *symbol, const char *reference, int id )
{
  return createDevice( device_lib_entry( symbol ), reference, id );
}

IDevice *SchemaSheet::createDevice( DeviceLibraryEntry *entry, const char *reference, int id )
{
  if( !entry ) return 0l;

  return entry->construct( reference, id, m_circuit, 0l );
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

}
