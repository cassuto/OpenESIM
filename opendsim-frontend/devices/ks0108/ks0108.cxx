/** @file
 * device-lib KS0108 LCD Driver
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

#include "ks0108.h"

DeviceLibraryEntry *dev_ks0108::libraryEntry()
{
  return new DeviceLibraryEntry(
      "ks0108",
      "128 x 64 LCD Controller and LCD",
      "DSIM Device",
      dev_ks0108::construct,
      0);
}

IDevice *dev_ks0108::construct( const char *reference, int id, circuit_t *circuit, void *reserved )
{
  return new dev_ks0108( reference, id, circuit, reserved );
}

dev_ks0108::dev_ks0108( const char *reference, int id, circuit_t *circuit, void *reserved )
  : IDevice( reference, id, circuit, reserved )
{
}

int dev_ks0108::init()
{
  return IDevice::baseinit( "d_ks0108" );
}

int dev_ks0108::render_frame( IDeviceGraph *graph )
{
  return 0;
}

int dev_ks0108::uninit()
{
  return 0;
}
