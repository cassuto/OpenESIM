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

#include <dsim/list.h>

#include "tsf/test.h"

typedef struct node_s
{
  DS_LIST_NODE();

  int value;
} node_t;

////////////////////////////////////////////////////////////////////////////////

static void *
my_copy( const void *src )
{
  node_t *node = (node_t*)ds_heap_alloc( sizeof(*node) );
  if ( node )
    {
      node->value = list_entry(src, node_t)->value;
    }
  return node;
}

int
main( int argc, char *argv[] )
{
  int elem_num = 1024;

  if ( ds_test_init( "test-list" ) ) return 1;

  node_t *node = NULL, *rootnode = NULL, *tailnode = NULL;
  list_t list, list2;
  int i = 0;

  list_init( &list );
  list_init( &list2 );

  for (int i = 0; i < elem_num; i++)
    {
      node = (node_t*)ds_heap_alloc(sizeof(*node));
      node->value = i;

      if ( i == 0 ) rootnode = node;
      if ( i == elem_num-1) tailnode = node;
      list_insert( &list, list_node(node) );
    }

  ds_test_check( !list_contains( &list, list_node(rootnode) ), "list_contain()" );
  ds_test_check( !list_contains( &list, list_node(tailnode) ), "list_contain()" );

  ds_test_check( list_copy( &list2, &list, ds_heap_free, my_copy ), "list_copy()" );

  ds_test_check( list_size( &list2 ) != elem_num, "list_copy(): wrong new list size" );

  i = 0;
  foreach_list( node_t, node, &list2 )
    {
      /* whether the node is NULL ? */
      ds_test_check( (node==NULL), "foreach_list()" );
      /* the element is right? */
      ds_test_check( ( node->value != i ), "list_copy(): wrong elements" );
      i++;
    }

  list_remove( &list, list_node(rootnode) );
  list_remove( &list, list_node(tailnode) );
  ds_heap_free( rootnode );
  ds_heap_free( tailnode );

  i = 0;
  foreach_list( node_t, node, &list )
    {
      /* whether the node is NULL ? */
      ds_test_check( (node==NULL), "foreach_list()" );

      /* deleted the root node correctly? */
      ds_test_check( ( (i == 0) && (node->value != 1) ), "list_delete() root node" );
      /* kept the right order? */
      ds_test_check( ( node->value != i+1 ), "foreach_list() disorder" );

      i++;
    }

  /* deleted the tail node correctly? */
  ds_test_check( ( i != elem_num-2 ), "list_delete() tail node" );

  /* query size */
  ds_test_check( ( list_size( &list ) != elem_num-2 ), "list_size()" );

  node = (node_t*)ds_heap_alloc(sizeof(*node));
  node->value = -1;
  list_insert( &list, list_node(node) );

  ds_test_check( ( list_size( &list ) != elem_num-1 ), "list_insert() || list_size()" );

  /* clear list */
  list_clear( &list, ds_heap_free );

  foreach_list( node_t, node, &list )
    ds_test_check( 1, "list_clear()" );

  return 0;
}
