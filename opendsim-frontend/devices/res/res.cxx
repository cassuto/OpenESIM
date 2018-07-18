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

#include "res.h"

DeviceLibraryEntry *dev_res::libraryEntry()
{
  return new DeviceLibraryEntry(
      /* symbol_name */     "RES",
      /* reference_name */  "R",
      /* category */        "Registers",
      /* sub_category */    "Generic",
      /* description */     "Generic Register Device",
      /* manufacturer */    "Registers",
      /* symbolfile */      "res.ssym",
      dev_res::construct,
      0l);
}

IDevice *dev_res::construct( const char *reference, int id, circuit_t *circuit, void *reserved )
{
  return new dev_res( reference, id, circuit, reserved );
}

dev_res::dev_res( const char *reference, int id, circuit_t *circuit, void *reserved )
  : IDevice( reference, id, circuit, reserved )
{
}

int dev_res::create()
{
  return 0;
}

int dev_res::init()
{
  return IDevice::baseinit( "register" );
}

int dev_res::render_frame( IDeviceGraph *graph )
{
  graph->setBrushColor( 255, 255, 255 );

#if 0
  static const QPointF points[3] = {
      QPointF( 7, 0 ),
      QPointF(-8,-7 ),
      QPointF(-8, 7 )              };

  graph->polygon(points, 3);
#endif

  graph->line( 7, 0, -8, -7 );
  graph->line( -8, -7, -8, 7 );
  graph->line( -8, 7, 7, 0 );

  graph->setPenWidth( 3 );

  graph->line( 7, -6, 7, 6 );
  return 0;
}

int dev_res::uninit()
{
  return 0;
}
