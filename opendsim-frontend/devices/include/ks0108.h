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

#ifndef DEVICE_KS0108_H_
#define DEVICE_KS0108_H_

#include "device-lib-internal.h"

class dev_ks0108 : public IDevice
{
public:
  dev_ks0108( const char *reference, int id, circuit_t *circuit, void *reserved );

  static DeviceLibraryEntry *libraryEntry();
  static IDevice *construct( const char *reference, int id, circuit_t *circuit, void *reserved );

  virtual int init();
  virtual int pin_count();
  virtual circ_pin_t * pin( int index );
  virtual int render_frame( IDeviceGraph *graph );
  virtual int uninit();
};

#endif //!defined(DEVICE_KS0108_H_)
