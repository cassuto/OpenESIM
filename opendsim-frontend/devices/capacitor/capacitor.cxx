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

#include "capacitor.h"

DeviceLibraryEntry *dev_capacitor::libraryEntry()
{
  return new DeviceLibraryEntry(
      /* symbol_name */     "CAP",
      /* reference_name */  "C?",
      /* category */        "Primitives",
      /* sub_category */    "Capacitors",
      /* description */     "Generic Capacitor Device",
      /* manufacturer */    "Capacitors",
      /* symbolfile */      "capacitor.ssym",
      dev_capacitor::construct,
      0l);
}

IDevice *dev_capacitor::construct( const char *reference, int id, void *reserved )
{
  return new dev_capacitor( reference, id, reserved );
}

dev_capacitor::dev_capacitor( const char *reference, int id, void *reserved )
  : IDevice( reference, id, reserved )
{}

int dev_capacitor::create( ISchematic *schematic, IPropertyContainer *properties )
{
  int rc = IDevice::baseinit( "C", m_circuit );                 UPDATE_RC(rc);
  rc = properties->readModel( m_mdel, 0 );                      UPDATE_RC(rc);

  UNUSED(schematic);
  return rc;
}

int dev_capacitor::init( ISchematic *, IPropertyContainer *properties )
{
  return properties->configModel( m_mdel );
}
