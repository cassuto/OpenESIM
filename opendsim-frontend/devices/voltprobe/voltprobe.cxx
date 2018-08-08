/** @file
 * device-lib DC Vsource
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

#include "voltprobe.h"

DeviceLibraryEntry *dev_voltprobe::libraryEntry()
{
  return new DeviceLibraryEntry(
      /* symbol_name */     "voltprobe",
      /* reference_name */  "vprobe?",
      /* category */        "Probes",
      /* sub_category */    "Generic",
      /* description */     "Generic voltage probe",
      /* manufacturer */    "Probes",
      /* symbolfile */      "voltprobe.ssym",
      dev_voltprobe::construct,
      0l);
}

IDevice *dev_voltprobe::construct( const char *reference, int id, void *reserved )
{
  return new dev_voltprobe( reference, id, reserved );
}

dev_voltprobe::dev_voltprobe( const char *reference, int id, void *reserved )
  : IDevice( reference, id, reserved )
{}

int dev_voltprobe::create( ISchematic *schematic, IPropertyContainer *properties )
{
  int rc = IDevice::baseinit( "VS", m_circuit );                UPDATE_RC(rc);
  rc = properties->readDevice( this );                          UPDATE_RC(rc);

  UNUSED(schematic);
  return rc;
}

int dev_voltprobe::init( ISchematic *, IPropertyContainer * )
{
  int rc = circ_element_config(( m_mdel, ELM_CONFIG_SET, /*Vh*/0, 0.0f ));  UPDATE_RC(rc);
      rc = circ_element_config(( m_mdel, ELM_CONFIG_SET, /*g*/2, (int)false )); UPDATE_RC(rc);
  return rc;
}
