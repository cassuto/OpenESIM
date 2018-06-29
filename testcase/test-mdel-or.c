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
  float error = 1e-6;
  int mode = 0;
  circ_matrix_t *matrix;
  circuit_t *circuit;
  if ( ds_test_init( "test-mdel-or" ) ) return 1;

  matrix = matrix_create();

  ds_test_check( (matrix == NULL), "circ_matrix_create()" );

  circuit = circuit_create( matrix );
  ds_test_check( (circuit == NULL), "circuit_create()" );

  /*
   * Build a simple RC circuit...
   */
  circ_element_t *R1, *OR1, *ADC1, *DAC1, *VS1, *VS2;
  R1 = circ_element_create( circuit, "R", 1 );
  ds_test_check( (R1 == NULL), "circ_element_create()" );
  OR1 = circ_element_create( circuit, "OR", 1 );
  ds_test_check( (OR1 == NULL), "circ_element_create()" );
  DAC1 = circ_element_create( circuit, "DAC", 1 );
  ds_test_check( (DAC1 == NULL), "circ_element_create()" );
  ADC1 = circ_element_create( circuit, "ADC", 1 );
  ds_test_check( (ADC1 == NULL), "circ_element_create()" );
  VS1 = circ_element_create( circuit, "VS", 1 );
  ds_test_check( (VS1 == NULL), "circ_element_create()" );
  VS2 = circ_element_create( circuit, "VS", 2 );
  ds_test_check( (VS2 == NULL), "circ_element_create()" );

  circuit_attach_element( circuit, VS1 );
  circuit_attach_element( circuit, VS2 );
  circuit_attach_element( circuit, DAC1 );
  circuit_attach_element( circuit, ADC1 );
  circuit_attach_element( circuit, R1 );
  circuit_attach_element( circuit, OR1 );

  circ_node_t *node[6];

  for(int i = 0; i<GET_ELEMENTS(node); i++)
    {
      if( i == 1 || i == 2 || i == 3) node[i] = circ_node_create( circuit, false );
      else node[i] = circ_node_create( circuit, true );

      ds_test_check( (node[i] == NULL), "circ_node_create()" );
      circuit_attach_node( circuit, node[i] );
    }

  ds_test_check( circ_element_config(( ADC1, ELM_CONFIG_SET, /*inc*/0, 2 )), "circ_element_config()" );

  circ_pin_set_node( VS1->pin_vector[0], node[0] );
  circ_pin_set_node( ADC1->pin_vector[0], node[0] );
  circ_pin_set_node( ADC1->pin_vector[1], node[5] );
  circ_pin_set_node( ADC1->pin_vector[2], node[1] );
  circ_pin_set_node( ADC1->pin_vector[3], node[2] );
  circ_pin_set_node( OR1->pin_vector[0], node[1] ); /* ina */
  circ_pin_set_node( OR1->pin_vector[1], node[2] ); /* inb */
  circ_pin_set_node( OR1->pin_vector[2], node[3] ); /* out */
  circ_pin_set_node( DAC1->pin_vector[0], node[3] );
  circ_pin_set_node( DAC1->pin_vector[1], node[4] );
  circ_pin_set_node( R1->pin_vector[0], node[4] );
  circ_pin_set_node( R1->pin_vector[1], node[5] );
  circ_pin_set_node( VS2->pin_vector[0], node[5] );

  ds_test_check( circ_element_config(( VS1, ELM_CONFIG_SET, /*Vh*/0, 5.0f ) ), "circ_element_config()" );
  ds_test_check( circ_element_config(( VS1, ELM_CONFIG_SET, /*g*/2, (int)true ) ), "circ_element_config()" );

  ds_test_check( circuit_init( circuit ), "circuit_init()" );

  ds_test_check( circuit_run_step( circuit ), "circuit_run_step()" );

  ds_test_check( SIG_HIGH != circ_pin_get_state( OR1->pin_vector[2] ), "wrong result" );
  ds_test_check_float( circ_node_get_volt( node[4] ), 5.0f, error, "wrong result" );

  ds_test_check( circ_element_config(( VS1, ELM_CONFIG_SET, /*g*/2, (int)false ) ), "circ_element_config()" );

  ds_test_check( circuit_run_step( circuit ), "circuit_run_step()" );

  ds_test_check( SIG_LOW != circ_pin_get_state( OR1->pin_vector[2] ), "wrong result" );
  ds_test_check_float( circ_node_get_volt( node[4] ), .0f, error, "wrong result" );

  circuit_free( circuit );

  matrix_free( matrix );

  return 0;
}
