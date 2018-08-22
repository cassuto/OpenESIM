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

#include "vsource-dc-0v.h"

DeviceLibraryEntry *dev_vsource_dc_zero::libraryEntry()
{
  return new DeviceLibraryEntry(
      /* symbol_name */     "vsource_dc_0v",
      /* reference_name */  "0V",
      /* category */        "Primitives",
      /* sub_category */    "Sources",
      /* description */     "DC zero voltage source",
      /* manufacturer */    "Sources",
      /* symbolfile */      "vsource_dc_0v.ssym",
      dev_vsource_dc_zero::construct,
      0l);
}

IDevice *dev_vsource_dc_zero::construct( const char *reference, int id, void *reserved )
{
  return new dev_vsource_dc_zero( reference, id, reserved );
}

dev_vsource_dc_zero::dev_vsource_dc_zero( const char *reference, int id, void *reserved )
  : IDevice( reference, id, reserved )
{}

int dev_vsource_dc_zero::create( ISchematic */*schematic*/, IPropertyContainer */*properties*/ )
{
  int rc = IDevice::baseinit( "VS", m_circuit );                UPDATE_RC(rc);
  return rc;
}

int dev_vsource_dc_zero::init( ISchematic *, IPropertyContainer *properties )
{
  return circ_element_config(( m_mdel, ELM_CONFIG_SET, /*Vh*/0, 0.0f ));
  UNUSED(properties);
}
