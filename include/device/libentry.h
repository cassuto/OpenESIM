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

#ifndef CIRC_LIBENTRY_H_
#define CIRC_LIBENTRY_H_

#if !defined(__cplusplus)
#error cplusplus only
#endif

#include <dsim/types.h>

class IDevice;

typedef IDevice *(*pfn_device_construct)( const char *reference, int id );

class DeviceLibraryEntry
{
public:
  DeviceLibraryEntry( const char *symbol_name_, const char *description_, const char *manufacturer_, pfn_device_construct construct_, void *reserved_ )
    : reserved( reserved_ ),
      symbol_name( symbol_name_ ),
      description( description_ ),
      manufacturer( manufacturer_ ),
      construct( construct_ )
  {}

public:
  void       *reserved;
  const char *symbol_name;
  const char *description;
  const char *manufacturer;
  pfn_device_construct construct;
};

#endif //!defined(CIRC_LIBENTRY_H_)
