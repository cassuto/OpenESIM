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

#ifndef INSTRUMENTLIB_H_
#define INSTRUMENTLIB_H_

#if !defined(__cplusplus)
#error cplusplus only
#endif

#include <dsim/types.h>
#include <dsim/list.h>
#include <dsim/hashmap.h>
#include <dsim/rbtree.h>

class InstrumentLibraryEntry;

typedef struct
{
  DS_RBTREE_NODE();

  const char *classname;
  InstrumentLibraryEntry *entry;
} inst_entry_search_node_t;

int instrument_lib_init();
rb_tree_t *instrument_lib_get_tree();
InstrumentLibraryEntry *instrument_lib_entry( const char *classname );

#endif
