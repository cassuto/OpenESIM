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

#ifndef DEVICE_1N4148_H_
#define DEVICE_1N4148_H_

#include "device-lib-internal.h"

class dev_1n4148 : public IDevice
{
public:
  dev_1n4148( const char *reference, int id, void *reserved );

  static DeviceLibraryEntry *libraryEntry();
  static IDevice *construct( const char *reference, int id, void *reserved );

  int create( ISchematic *schematic, IPropertyContainer *properties );
  int init( IPropertyContainer *properties );
};

#endif //!defined(DEVICE_1N4148_H_)
