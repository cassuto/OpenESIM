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

#include "inductor.h"

DeviceLibraryEntry *dev_inductor::libraryEntry()
{
  return new DeviceLibraryEntry(
      /* symbol_name */     "INDUCTOR",
      /* reference_name */  "L?",
      /* category */        "Primitives",
      /* sub_category */    "Inductors",
      /* description */     "Generic Inductor Device",
      /* manufacturer */    "Inductors",
      /* symbolfile */      "inductor.ssym",
      dev_inductor::construct,
      0l);
}

IDevice *dev_inductor::construct( const char *reference, int id, void *reserved )
{
  return new dev_inductor( reference, id, reserved );
}

dev_inductor::dev_inductor( const char *reference, int id, void *reserved )
  : IDevice( reference, id, reserved )
{}

int dev_inductor::create( ISchematic *schematic, IPropertyContainer *properties )
{
  int rc = IDevice::baseinit( "L", m_circuit );                 UPDATE_RC(rc);
  rc = properties->readModel( m_mdel, 0 );                      UPDATE_RC(rc);
  rc = properties->readDevice( this );                          UPDATE_RC(rc);

  UNUSED(schematic);
  return rc;
}

int dev_inductor::init( ISchematic *, IPropertyContainer *properties )
{
  return properties->configModel( m_mdel );
}
