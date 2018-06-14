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

#include <dsim/hashmap.h>

#include "tsf/test.h"

typedef struct node_s
{
  DS_HASHMAP_NODE();

  int value;
} node_t;

////////////////////////////////////////////////////////////////////////////////


int
main( int argc, char *argv[] )
{
  int elem_num = 1024, count = 0;

  if ( ds_test_init( "test-hashmap" ) ) return 1;

  node_t *node = NULL;
  hashmap_t map;

  ds_test_check( hashmap_init( &map, HASHMAP_KEY_INTPTR, 3 ), "hashmap_create()" );

  for (int i = 0; i < elem_num; i++)
    {
      node = (node_t*)ds_heap_alloc(sizeof(*node));
      node->value = i;

      hashmap_insert( &map, i, hashmap_node(node) );
    }

  int k = 0;
  foreach_hashmap( node_t, node, &map )
    {
      ds_test_check( !node, "foreach_hashmap()" );
      k++;
    }
  ds_test_check( k != elem_num, "foreach_hashmap(): element lost" );

  hashmap_remove( &map, elem_num-1, ds_heap_free );
  hashmap_remove( &map, 7, ds_heap_free );

  for (int i = 0; i < elem_num; i++)
    {
      node = hashmap_entry(hashmap_at( &map, i ), node_t);

      if ( i == (elem_num-1) || i == 7 )
        {
          /* whether the node is NULL ? */
          ds_test_check( (node!=NULL), "hashmap_remove()" );
        }
      else if ( node )
        {
          /* whether the node is right ? */
          ds_test_check( (node->value != i), "hashmap_at()" );
          count++;
        }
      else
        {
          ds_test_check( 1, "null output from hashmap_at()" );
        }
    }

  ds_test_check( (count++ != elem_num-2), "hashmap_remove()" );

  node = (node_t*)ds_heap_alloc(sizeof(*node));
  node->value = 20;
  hashmap_insert( &map, 7, hashmap_node(node) );

  node = hashmap_entry(hashmap_at( &map, 7 ), node_t);
  ds_test_check( !(node && node->value == 20), "hashmap_at() after hashmap_insert()" );

  hashmap_clear( &map, ds_heap_free );

  hashmap_uninit( &map );

  /*
   * testing string hash
   */
  hashmap_init( &map, HASHMAP_KEY_CHR, 3 );

  static const char *keys[8] =
      {
          "abchdhfj",
          "fkieutf",
          "ocmdff"
          "~@E%&*()(*&^%",
          "iwjdmfm",
          "adcvnef",
          "eireferj",
          "oedmccm",
          "039493958"
      };

  for (int i = 0; i < 8; i++)
    {
      node = (node_t*)ds_heap_alloc(sizeof(*node));
      node->value = i;

      hashmap_insert( &map, (hashmap_key_t)keys[i], hashmap_node(node) );
    }

  for (int i = 0; i < 8; i++)
    {
      node = hashmap_entry(hashmap_at( &map, (hashmap_key_t)keys[i] ), node_t);

      ds_test_check( !(node && node->value == i), "hashmap_at()" );
    }

  hashmap_clear( &map, ds_heap_free );

  hashmap_uninit( &map );

  return 0;
}
