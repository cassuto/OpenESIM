/** @file
 * device-lib
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

#define TRACE_UNIT "device-lib"

#include <dsim/logtrace.h>
#include <dsim/node-template.h>
#include <dsim/list.h>

#include "ks0108.h"

static list_t device_list;

#define Entry(ptr) list_insert_voidptr( &device_list, ptr )

int device_lib_init()
{
  list_init( &device_list );

  Entry( dev_ks0108::libraryEntry() );
  return 0;
}

const list_t *device_lib_get_list()
{
  return &device_list;
}

void device_lib_uninit()
{
  list_release( &device_list, ds_heap_free );
}
