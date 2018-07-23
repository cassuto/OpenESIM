/*
 *  OpenDSIM (A/D mixed circuit simulator)
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
#include <model/circ-node.h>
#include <model/circ-element.h>
#include <dsim/circmatrix.h>

#include "tsf/test.h"



int
main( int argc, char *argv[] )
{
  if ( ds_test_init( "test-circmatrix" ) ) return 1;

  int elem_num = 1024, count = 0, rc;
  list_t node_list;
  list_t element_list;
  float v[3];
  float error = 1e-3;
  int i = 0;

  circ_matrix_t *matrix = matrix_create();

  ds_test_check( (matrix == NULL), "circ_matrix_create()" );

  list_init( &node_list );
  list_init( &element_list );

  for( int i=0; i< 3; i++)
    {
      circ_node_t *node = circ_node_create( NULL, true );
      ds_test_check( (node == NULL), "circ_node_create()" );

      circ_node_init( node );
      list_insert( &node_list, list_node(node) );
    }

  rc = matrix_init( matrix, &node_list );
  ds_test_check( rc, "matrix_init()" );

  matrix_stamp( matrix, 1, 1, 1.0f );
  matrix_stamp( matrix, 1, 2, 3.0f );
  matrix_stamp( matrix, 1, 3, 3.0f );
  matrix_stamp_coef( matrix, 1, 5.0f );

  matrix_stamp( matrix, 2, 1, 2.0f );
  matrix_stamp( matrix, 2, 2, -1.0f );
  matrix_stamp( matrix, 2, 3, 4.0f );
  matrix_stamp_coef( matrix, 2, 11.0f );

  matrix_stamp( matrix, 3, 1, 0.0f );
  matrix_stamp( matrix, 3, 2, -1.0f );
  matrix_stamp( matrix, 3, 3, 1.0f );
  matrix_stamp_coef( matrix, 3, 3.0f );

  matrix_print( matrix );

  ds_test_check( matrix_solve( matrix ), "matrix_solve()\n" );

  i = 0;
  foreach_list( circ_node_t, node, &node_list )
    {
      v[i] = circ_node_get_volt( node );
      trace_info(("%d volt %f\n", i, v[i] ));
      i++;
    }

  ds_test_check_float(        v[0] + 3.0f  * v[1] + 3.0f * v[2], 5.0f, error, "matrix_solve() wrong result" );
  ds_test_check_float( 2.0f * v[0] + -1.0f * v[1] + 4.0f * v[2], 11.0f, error, "matrix_solve() wrong result" );
  ds_test_check_float(               -1.0f * v[1] +        v[2], 3.0f, error, "matrix_solve() wrong result" );

  matrix_free( matrix );

  return 0;
}
