/** @file
 * device-lib Register
 */

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

#include "xor.h"

DeviceLibraryEntry *dev_xor::libraryEntry()
{
  return new DeviceLibraryEntry(
      /* symbol_name */     "XOR",
      /* reference_name */  "U?",
      /* category */        "Primitives",
      /* sub_category */    "Logical gates",
      /* description */     "Generic XOR Gate Device",
      /* manufacturer */    "XOR",
      /* symbolfile */      "xor.ssym",
      dev_xor::construct,
      0l);
}

IDevice *dev_xor::construct( const char *reference, int id, void *reserved )
{
  return new dev_xor( reference, id, reserved );
}

dev_xor::dev_xor( const char *reference, int id, void *reserved )
  : IDevice( reference, id, reserved )
{}

int dev_xor::create( ISchematic *schematic, IPropertyContainer *properties )
{
  int rc = IDevice::baseinit( "XOR", m_circuit );               UPDATE_RC(rc);
  rc = properties->readDevice( this );                          UPDATE_RC(rc);

  UNUSED(schematic);
  return rc;
}

int dev_xor::init( ISchematic *, IPropertyContainer *properties )
{
  return properties->configDevice( this );
}
