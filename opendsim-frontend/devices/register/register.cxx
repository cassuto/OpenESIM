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

#include "register.h"

DeviceLibraryEntry *dev_register::libraryEntry()
{
  return new DeviceLibraryEntry(
      /* symbol_name */     "RES",
      /* reference_name */  "R?",
      /* category */        "Primitives",
      /* sub_category */    "Registers",
      /* description */     "Generic Register Device",
      /* manufacturer */    "Registers",
      /* symbolfile */      "register.ssym",
      dev_register::construct,
      0l);
}

IDevice *dev_register::construct( const char *reference, int id, void *reserved )
{
  return new dev_register( reference, id, reserved );
}

dev_register::dev_register( const char *reference, int id, void *reserved )
  : IDevice( reference, id, reserved )
{}

int dev_register::create( ISchematic *schematic, IPropertyContainer *properties )
{
  int rc = IDevice::baseinit( "R", m_circuit );                 UPDATE_RC(rc);
  rc = properties->readModel( m_mdel, 0 );                      UPDATE_RC(rc);

  UNUSED(schematic);
  return rc;
}

int dev_register::init( ISchematic *, IPropertyContainer *properties )
{
  return properties->configModel( m_mdel );
}
