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
#include <dsim/circmatrix.h>
#include <dsim/circuit.h>
#include <model/model.h>
#include <model/circ-pin.h>
#include <model/circ-node.h>
#include <model/circ-element.h>

#include "tsf/test.h"



int
main( int argc, char *argv[] )
{
  int rc;
  if ( ds_test_init( "test-circuit" ) ) return 1;

  circ_matrix_t *matrix = matrix_create();

  ds_test_check( (matrix == NULL), "circ_matrix_create()" );

  circuit_t *circuit = circuit_create( matrix );
  ds_test_check( (circuit == NULL), "circuit_create()" );

  /*
   * Build a simple RC circuit...
   */
  circ_element_t *C1, *R1, *VS1, *VS2;
  C1 = circ_element_create( circuit, "C", 1 );
  ds_test_check( (C1 == NULL), "circ_element_create()" );
  R1 = circ_element_create( circuit, "R", 1 );
  ds_test_check( (R1 == NULL), "circ_element_create()" );
  VS1 = circ_element_create( circuit, "VS", 1 );
  ds_test_check( (VS1 == NULL), "circ_element_create()" );
  VS2 = circ_element_create( circuit, "VS", 2 );
  ds_test_check( (VS2 == NULL), "circ_element_create()" );

  circuit_attach_element( circuit, C1 );
  circuit_attach_element( circuit, R1 );
  circuit_attach_element( circuit, VS1 );
  circuit_attach_element( circuit, VS2 );

  circ_node_t *node[3];

  for(int i = 0; i<GET_ELEMENTS(node); i++)
    {
      node[i] = circ_node_create( circuit, true );
      ds_test_check( (node[i] == NULL), "circ_node_create()" );
      circuit_attach_node( circuit, node[i] );
    }

  circ_pin_set_node( VS1->pin_vector[0], node[0] );
  circ_pin_set_node( R1->pin_vector[0], node[0] );
  circ_pin_set_node( R1->pin_vector[1], node[1] );
  circ_pin_set_node( C1->pin_vector[0], node[1] );
  circ_pin_set_node( C1->pin_vector[1], node[2] );
  circ_pin_set_node( VS2->pin_vector[0], node[2] );

  ds_test_check( circ_element_config(( VS1, ELM_CONFIG_SET, /*Vh*/0, 5.0f ) ), "circ_element_config()" );
  ds_test_check( circ_element_config(( VS1, ELM_CONFIG_SET, /*g*/2, (int)true ) ), "circ_element_config()" );

  ds_test_check( circ_element_config(( R1, ELM_CONFIG_SET, /*r*/0, 10.0f ) ), "circ_element_config()" );
  ds_test_check( circ_element_config(( C1, ELM_CONFIG_SET, /*c*/0, 1.0f ) ), "circ_element_config()" );

  ds_test_check( circuit_init( circuit ), "circuit_init()" );

//  FILE *fdump = fopen("circuit0_plot.txt", "w");
//  ds_test_check( fdump==NULL, "fopen()" );

  double last = .0f;
  for(int i=0;i<60;i++)
    {
      ds_test_check( circuit_run_step( circuit ), "circuit_run_step()" );
      ds_test_check( last >= circ_node_get_volt( node[1] ), "not RC behavior" );
      last = circ_node_get_volt( node[1] );
//      fprintf( fdump, "%f\n", circ_node_get_volt( node[1] ) );
    }
//  fclose(fdump);

  circuit_free( circuit );

  matrix_free( matrix );

  return 0;
}
