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

#ifndef DEVICE_SCHEMATIC_H_
#define DEVICE_SCHEMATIC_H_

class IDeviceGraph;
class IDevice;
class ISchematic
{
public:
  ISchematic() {}
  virtual ~ISchematic() {}

  virtual void changeValue( const char *value )=0;
  virtual IDeviceGraph *getDeviceGraph( const char *tokenId )=0;
  virtual void registerRender( IDevice *device )=0; // called within IDevice::init(), not create()
};

#endif
