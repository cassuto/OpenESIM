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

#include "buff.h"

DeviceLibraryEntry *dev_buff::libraryEntry()
{
  return new DeviceLibraryEntry(
      /* symbol_name */     "BUFF",
      /* reference_name */  "U?",
      /* category */        "Primitives",
      /* sub_category */    "Logical devices",
      /* description */     "Generic buffer Device",
      /* manufacturer */    "BUFF",
      /* symbolfile */      "buff.ssym",
      dev_buff::construct,
      0l);
}

IDevice *dev_buff::construct( const char *reference, int id, void *reserved )
{
  return new dev_buff( reference, id, reserved );
}

dev_buff::dev_buff( const char *reference, int id, void *reserved )
  : IDevice( reference, id, reserved )
{}

int dev_buff::create( ISchematic *schematic, IPropertyContainer *properties )
{
  int rc = IDevice::baseinit( "BUFF", m_circuit );              UPDATE_RC(rc);
      rc = properties->readDevice( this );                          UPDATE_RC(rc);

  UNUSED(schematic);
  return rc;
}

int dev_buff::init( ISchematic *, IPropertyContainer *properties )
{
  return properties->configDevice( this );
}
