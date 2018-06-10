/*
 *  OpenDSIM (Opensource Digital Circuit Simulation)
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
  int elem_num = 1024;

  if ( ds_test_init( "test-hashmap" ) ) return 1;

  node_t *node = NULL;
  hashmap_t *map = hashmap_create( HASHMAP_KEY_INT, 3 );

  ds_test_check( map==NULL, "hashmap_create()" );

  for (int i = 0; i < elem_num; i++)
    {
      node = (node_t*)ds_heap_alloc(sizeof(*node));
      node->value = i;

      hashmap_insert( map, i, node );
    }

  hashmap_remove( map, 5, ds_heap_free );
  hashmap_remove( map, 7, ds_heap_free );

  for (int i = 0; i < elem_num; i++)
    {
      node = hashmap_at( map, i );

      if ( i == 5 || i == 7 )
        {
          /* whether the node is NULL ? */
          ds_test_check( !(node==NULL), "hashmap_remove()" );
        }
      else if ( node )
        {
          /* whether the node is right ? */
          ds_test_check( !(node && node->value == i), "hashmap_at()" );
        }
      else
        {
          ds_test_check( 1, "null output from hashmap_at()" );
        }
    }

  node = (node_t*)ds_heap_alloc(sizeof(*node));
  node->value = 20;
  hashmap_insert( map, 7, node );

  node = hashmap_at( map, 7 );
  ds_test_check( !(node && node->value == 20), "hashmap_at() after hashmap_insert()" );

  hashmap_free( map, ds_heap_free );

  /*
   * testing string hash
   */
  map = hashmap_create( HASHMAP_KEY_CHR, 3 );

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

      hashmap_insert( map, (hashmap_key_t)keys[i], node );
    }

  for (int i = 0; i < 8; i++)
    {
      node = hashmap_at( map, (hashmap_key_t)keys[i] );

      ds_test_check( !(node && node->value == i), "hashmap_at()" );
    }

  hashmap_free( map, ds_heap_free );

  return 0;
}
