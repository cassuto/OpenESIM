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
#include <dsim/misc.h>
#include <dsim/node-template.h>
#include <dsim/list.h>
#include <dsim/hashmap.h>
#include <dsim/rbtree.h>

#include <cstring>

#include "1n4148.h"
#include "diode.h"
#include "vsource-dc.h"
#include "vsource-dc-0v.h"
#include "register.h"
#include "capacitor.h"
#include "inductor.h"
#include "voltprobe.h"

#include <frontend/device-lib.h>

static hashmap_t device_categories;     /* category_node */
static rb_tree_t device_tree;           /* device_entry_search_node */

static int device_cmp( const rb_node_t *left, const rb_node_t *right )
{
  device_entry_search_node_t *lnode = rb_entry(left, device_entry_search_node_t);
  device_entry_search_node_t *rnode = rb_entry(right, device_entry_search_node_t);

  return std::strcmp( lnode->symbol_name, rnode->symbol_name );
}

static int Entry( DeviceLibraryEntry *entry)
{
  int rc;

  if( entry )
    {
      /*
       * Simply categorize the device according its category and sub-category
       */
      category_node_t *node = hashmap_cast_at( &device_categories, (hashmap_key_t)entry->category, category_node_t );
      if( !node )
        {
          node = hashmap_alloc_node( &device_categories, category_node_t );
          if( node )
            {
              std::memset( node, 0, sizeof(*node) );
              node->name = entry->category;
              if( (rc = hashmap_init( &node->sub_categories, HASHMAP_KEY_CHR, 0 )) )
                {
                  hashmap_collect( &device_categories, node );
                  return rc;
                }
              hashmap_insert( &device_categories, (hashmap_key_t)entry->category, hashmap_node(node) );
            }
        }
      if( node )
        {
          sub_category_node_t *sub_node = hashmap_cast_at( &node->sub_categories, (hashmap_key_t)entry->sub_category, sub_category_node_t );
          if( !sub_node )
            {
              sub_node = hashmap_alloc_node( &node->sub_categories, sub_category_node_t );
              if( sub_node )
                {
                  std::memset( sub_node, 0, sizeof(*sub_node) );
                  sub_node->name = entry->sub_category;
                  list_init( &sub_node->entries );
                  hashmap_insert( &node->sub_categories, (hashmap_key_t)entry->sub_category, hashmap_node(sub_node) );
                }
            }
          if( sub_node )
            {
              device_entry_node_t *entry_node = list_alloc_node( &sub_node->entries, device_entry_node_t );
              if( entry_node )
                {
                  entry_node->entry = entry;
                  list_insert( &sub_node->entries, list_node(entry_node) );

                  /*
                   * Insert to the device to search tree
                   */
                  device_entry_search_node_t *devnode = (device_entry_search_node_t *)ds_heap_alloc( sizeof(*devnode) );
                  if( devnode )
                    {
                      devnode->symbol_name = entry->symbol_name;
                      devnode->entry = entry;
                      rb_add( &device_tree, rb_node(devnode), device_cmp );
                      return 0;
                    }
                }
            }
        }
    }
  return -DS_NO_MEMORY;
}

int device_lib_init()
{
  int rc = 0;

  if( (rc = hashmap_init( &device_categories, HASHMAP_KEY_CHR, 0 )) )
    return rc;

  rb_init( &device_tree );

  rc = Entry( dev_1n4148::libraryEntry() );     UPDATE_RC(rc);
  rc = Entry( dev_diode::libraryEntry() );      UPDATE_RC(rc);
  rc = Entry( dev_vsource_dc::libraryEntry() ); UPDATE_RC(rc);
  rc = Entry( dev_vsource_dc_zero::libraryEntry() ); UPDATE_RC(rc);
  rc = Entry( dev_voltprobe::libraryEntry() );  UPDATE_RC(rc);
  rc = Entry( dev_register::libraryEntry() );   UPDATE_RC(rc);
  rc = Entry( dev_capacitor::libraryEntry() );  UPDATE_RC(rc);
  rc = Entry( dev_inductor::libraryEntry() );   UPDATE_RC(rc);

  return 0;
}

const hashmap_t *device_lib_get_categories()
{
  return &device_categories;
}
rb_tree_t *device_lib_get_tree()
{
  return &device_tree;
}

DeviceLibraryEntry *device_lib_entry( const char *symbol )
{
  device_entry_search_node_t value;
  value.symbol_name = symbol;

  rb_node_t *find = rb_find( &device_tree, rb_node(&value), device_cmp );
  if( find )
    {
      return rb_entry(find, device_entry_search_node_t)->entry;
    }
  return 0l;
}
