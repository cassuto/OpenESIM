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

#include "and.h"

DeviceLibraryEntry *dev_and::libraryEntry()
{
  return new DeviceLibraryEntry(
      /* symbol_name */     "AND",
      /* reference_name */  "U?",
      /* category */        "Primitives",
      /* sub_category */    "Logical gates",
      /* description */     "Generic AND Gate Device",
      /* manufacturer */    "AND",
      /* symbolfile */      "and.ssym",
      dev_and::construct,
      0l);
}

IDevice *dev_and::construct( const char *reference, int id, void *reserved )
{
  return new dev_and( reference, id, reserved );
}

dev_and::dev_and( const char *reference, int id, void *reserved )
  : IDevice( reference, id, reserved )
{}

int dev_and::create( ISchematic *schematic, IPropertyContainer *properties )
{
  int rc = IDevice::baseinit( "AND", m_circuit );               UPDATE_RC(rc);
  rc = properties->readDevice( this );                          UPDATE_RC(rc);

  UNUSED(schematic);
  return rc;
}

int dev_and::init( ISchematic *, IPropertyContainer *properties )
{
  return properties->configDevice( this );
}
