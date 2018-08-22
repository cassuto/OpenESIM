/** @file
 * instrument-lib
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

#define TRACE_UNIT "inst-lib"

#include <dsim/logtrace.h>
#include <dsim/misc.h>
#include <dsim/node-template.h>
#include <dsim/list.h>
#include <dsim/hashmap.h>
#include <dsim/rbtree.h>
#include <cstring>

#include <instrument/instrumentbase.h>
#include <instrument/libentry.h>

#include "inst_oscilloscope.h"
#include "inst_plotter.h"

#include <frontend/instrument-lib.h>

static rb_tree_t instrument_tree;       /* inst_entry_search_node */

static int inst_cmp( const rb_node_t *left, const rb_node_t *right )
{
  inst_entry_search_node_t *lnode = rb_entry(left, inst_entry_search_node_t);
  inst_entry_search_node_t *rnode = rb_entry(right, inst_entry_search_node_t);

  return std::strcmp( lnode->classname, rnode->classname );
}

static int Entry( InstrumentLibraryEntry *entry)
{
  if( entry )
    {
      /*
       * Insert to the device to search tree
       */
      inst_entry_search_node_t *instnode = (inst_entry_search_node_t *)ds_heap_alloc( sizeof(*instnode) );
      if( instnode )
        {
          instnode->classname = entry->classname;
          instnode->entry = entry;
          rb_add( &instrument_tree, rb_node(instnode), inst_cmp );
          return 0;
        }
    }
  return -DS_NO_MEMORY;
}

int instrument_lib_init()
{
  int rc = 0;

  rb_init( &instrument_tree );

  rc = Entry( inst_oscilloscope::libraryEntry() );  UPDATE_RC(rc);
  rc = Entry( inst_plotter::libraryEntry() );       UPDATE_RC(rc);
  return 0;
}

rb_tree_t *instrument_lib_get_tree()
{
  return &instrument_tree;
}

InstrumentLibraryEntry *instrument_lib_entry( const char *classname )
{
  inst_entry_search_node_t value;
  value.classname = classname;

  rb_node_t *find = rb_find( &instrument_tree, rb_node(&value), inst_cmp );
  if( find )
    {
      return rb_entry(find, inst_entry_search_node_t)->entry;
    }
  return 0l;
}
