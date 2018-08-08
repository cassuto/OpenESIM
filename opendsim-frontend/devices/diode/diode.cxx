/** @file
 * device-lib 1N4148 Diode
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

#include "diode.h"

DeviceLibraryEntry *dev_diode::libraryEntry()
{
  return new DeviceLibraryEntry(
      /* symbol_name */     "DIODE",
      /* reference_name */  "D?",
      /* category */        "Primitives",
      /* sub_category */    "Diodes",
      /* description */     "Generic Diode Device",
      /* manufacturer */    "Diodes",
      /* symbolfile */      "diode.ssym",
      dev_diode::construct,
      0l);
}

IDevice *dev_diode::construct( const char *reference, int id, void *reserved )
{
  return new dev_diode( reference, id, reserved );
}

dev_diode::dev_diode( const char *reference, int id, void *reserved )
  : IDevice( reference, id, reserved )
{}

int dev_diode::create( ISchematic *schematic, IPropertyContainer *properties )
{
  int rc = IDevice::baseinit( "D", m_circuit );                 UPDATE_RC(rc);
  rc = properties->readModel( m_mdel );                         UPDATE_RC(rc);
  rc = properties->readDevice( this );                          UPDATE_RC(rc);

  UNUSED(schematic);
  return rc;
}

int dev_diode::init( ISchematic *, IPropertyContainer *properties )
{
  return properties->configModel( m_mdel );
}
