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

int SchemaSheet::allocId()
{
  using namespace std;

  int num = 0, delta = 0;

  sort( m_ids.begin(), m_ids.end(), less<int>() );

  for( vector<int>::iterator n = m_ids.begin(); n!=m_ids.end(); n++ )
    {
      delta = *n - num;
      if( delta > 1 )
        {
          num = (*n) - delta;
          break;
        }
      num = *n;
    }

  ++num;

  m_ids.push_back( num );
  return num;
}

void SchemaSheet::releaseId( int id )
{
  using namespace std;

  vector<int>::iterator it = std::find( m_ids.begin(), m_ids.end(), id );
  if( it != m_ids.end() )
    {
      m_ids.erase( it );
    }
}

void SchemaSheet::addId( int id )
{
  if( idUnused( id ) )
    {
      m_ids.push_back( id );
    }
}

bool SchemaSheet::idUnused( int id )
{
  return m_ids.end() == std::find( m_ids.begin(), m_ids.end(), id );
}

void SchemaSheet::clearId()
{
  m_ids.clear();
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
