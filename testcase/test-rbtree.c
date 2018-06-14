/*
 *  OpenDSIM (Opensource Circuit Simulator)
 *  Copyright (C) 2016, The first Middle School in Yongsheng Lijiang China
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

#include <stdlib.h>
#include <stdio.h>

#define TRACE_UNIT "tsf"

#include <dsim/logtrace.h>
#include <dsim/string.h>
#include <dsim/memory.h>

#include <dsim/rbtree.h>

#include "tsf/test.h"

typedef struct node_s
{
  DS_RBTREE_NODE();

  int value;
} node_t;

////////////////////////////////////////////////////////////////////////////////

static int
int_cmp(const rb_node_t *left, const rb_node_t *right)
{
  node_t *lnode = rb_entry(left, node_t);
  node_t *rnode = rb_entry(right, node_t);

  return lnode->value - rnode->value;
}

int
main( int argc, char *argv[] )
{
  int elem_num = 6;
  int values[] = {1,3,5,2,4,6};

  rb_tree_t tree;
  node_t *node, *find = NULL;

  if ( ds_test_init( "test-rbtree" ) ) return 1;

  rb_init(&tree);

  for(int i = 0; i<elem_num; i++)
    {
      node = (node_t*)ds_heap_alloc(sizeof(*node));
      node->value = values[i];

      if ( i == 2 ) find = node;

      rb_add( &tree, rb_node(node), int_cmp );
    }

  int i = 1;
  foreach_rbtree(node_t, node, &tree)
    {
      ds_test_check( rb_entry(node, node_t)->value != i, "foreach_rbtree() || rbtree broken" );
      i++;
    }

  node = rb_entry(rb_find( &tree, rb_node(find), int_cmp), node_t);

  ds_test_check( !node || node->value != 5, "rb_find()" );

  rb_remove( &tree, rb_node(node) );
  ds_heap_free( node );

  foreach_rbtree(node_t, node, &tree)
    {
      ds_test_check( rb_entry(node, node_t)->value == 5, "rb_remove()" );
    }

  return 0;
}
