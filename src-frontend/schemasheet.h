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

#include <dsim/circmatrix.h>
#include <dsim/circuit.h>

class IDevice;
class DeviceLibraryEntry;

namespace dsim
{

class SchemaSheet
{
public:
  SchemaSheet();
  ~SchemaSheet();

  IDevice *createDevice( const char *symbol, const char *reference, int id );
  IDevice *createDevice( DeviceLibraryEntry *entry, const char *reference, int id );
  void deleteDevice( IDevice *device );

  void addDevice( IDevice *device );
  void removeDevice( IDevice *device );

private:
  circ_matrix_t *m_matrix;
  circuit_t *m_circuit;
};

}

#endif //!defined(SCHEMASHEET_H_)
