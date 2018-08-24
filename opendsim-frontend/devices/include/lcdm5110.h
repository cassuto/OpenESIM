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

#ifndef DEVICE_LCDM5110_H_
#define DEVICE_LCDM5110_H_

#include "device-lib-internal.h"

class dev_lcdm5110 : public IDevice
{
public:
  dev_lcdm5110( const char *reference, int id, void *reserved );

  static DeviceLibraryEntry *libraryEntry();
  static IDevice *construct( const char *reference, int id, void *reserved );

  int create( ISchematic *schematic, IPropertyContainer *properties );
  int init( ISchematic *schematic, IPropertyContainer *properties );
  int config( int op, ... );
  int render_frame( ISchematic *schematic, IDeviceGraph *deviceGraph );

private:
  const unsigned char (*m_gdram)[6][84];
};

#endif
