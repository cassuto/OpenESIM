/** @file
 * Hash map implement
 */

/*
 *  OpenDSIM (Opensource Circuit Simulator)
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

#include <dsim/misc.h>
#include <dsim/memory.h>
#include <dsim/error.h>

#include <dsim/list.h>

////////////////////////////////////////////////////////////////////////////////

void
list_clear( list_t *list, pfn_free_elem free_elem )
{
  if ( free_elem )
    {
      list_node_t *node = list->root, *cur;
      while ( node )
        {
          cur = node;
          node = cur->next;
          free_elem( cur );
        }
    }
  list->root = list->tail = NULL;
}

/* free_elem is optional, null if not delete */
int
list_copy( list_t *dst, const list_t *src, pfn_free_elem free_elem, pfn_copy_elem copy_elem )
{
  void *dup;

  list_clear( dst, free_elem );

  foreach_list( void, node, src )
    {
      dup = copy_elem( node );
      if (!dup)
        return -DS_FAULT;
      list_insert( dst, list_node(dup) );
    }
  return 0;
}
