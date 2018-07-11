/** @file
 * Hash map implement
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

#include <dsim/misc.h>
#include <dsim/memory.h>
#include <dsim/error.h>

#include <dsim/list.h>


/* if free_elem==NULL then collect the memory for the next allocation */
void
list_clear( list_t *list, pfn_free_elem free_elem )
{
  if( free_elem )
    { /* free memory of each element with free_elem() */
      register list_node_t *node = list->root, *cur;
      while ( node )
        {
          cur = node;
          node = cur->next;
          free_elem( cur );
        }
    }
  /* memory collector: just collect the whole list without free */
  else if( list->root )
    {
      list->tail->next = list->collected_root;
      list->collected_root = list->root;
    }
  list->root = list->tail = NULL;
}

/* free_elem is optional, null if collect and not delete the original one */
int
list_copy( list_t *dst, const list_t *src, pfn_free_elem free_elem, pfn_copy_elem copy_elem )
{
  void *dup;

  list_clear( dst, free_elem );

  foreach_list( void, node, src )
    {
      dup = copy_elem( dst, node );
      if (!dup)
        return -DS_FAULT;
      list_insert( dst, list_node(dup) );
    }
  return 0;
}

void
list_release( list_t *list, pfn_free_elem free_elem )
{
  /* free memory of collected elements */
  list_node_t *node = list->collected_root, *cur;
  while( node )
    {
      cur = node;
      node = node->next;

      free_elem( cur );
    }
  list->collected_root = NULL;

  /* free memory of elements in list */
  list_clear( list, free_elem );
}
