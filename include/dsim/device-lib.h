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

#ifndef DEVICE_LIB_H_
#define DEVICE_LIB_H_

#if !defined(__cplusplus)
#error cplusplus only
#endif

#include <dsim/types.h>
#include <dsim/list.h>
#include <dsim/hashmap.h>
#include <dsim/rbtree.h>

#include <device/device.h>
#include <device/libentry.h>

typedef struct
{
  DS_LIST_NODE();

  DeviceLibraryEntry *entry;
} device_entry_node_t;

typedef struct
{
  DS_RBTREE_NODE();

  const char *symbol_name;
  DeviceLibraryEntry *entry;
} device_entry_search_node_t;

typedef struct
{
  DS_HASHMAP_NODE();

  const char   *name;
  hashmap_t     sub_categories; /* sub_category_node */
} category_node_t;

typedef struct
{
  DS_HASHMAP_NODE();

  const char   *name;
  list_t        entries; /* device_entry_node */
} sub_category_node_t;


int device_lib_init();
const hashmap_t *device_lib_get_categories();
rb_tree_t *device_lib_get_tree();
DeviceLibraryEntry *device_lib_entry( const char *symbol );

#endif //!defined(DEVICE_LIB_H_)
