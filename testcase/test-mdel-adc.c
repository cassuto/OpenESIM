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
  if ( ds_test_init( "test-mdel-adc" ) ) return 1;

  matrix = matrix_create();

  ds_test_check( (matrix == NULL), "circ_matrix_create()" );

build_circuit:
  circuit = circuit_create( matrix );
  ds_test_check( (circuit == NULL), "circuit_create()" );

  /*
   * Build a simple RC circuit...
   */
  circ_element_t *ADC1, *VS1, *VS2;
  ADC1 = circ_element_create( circuit, "ADC", 1 );
  ds_test_check( (ADC1 == NULL), "circ_element_create()" );
  VS1 = circ_element_create( circuit, "VS", 1 );
  ds_test_check( (VS1 == NULL), "circ_element_create()" );
  VS2 = circ_element_create( circuit, "VS", 2 );
  ds_test_check( (VS2 == NULL), "circ_element_create()" );

  circuit_attach_element( circuit, ADC1 );
  circuit_attach_element( circuit, VS1 );
  if( mode == 0) circuit_attach_element( circuit, VS2 );

  if( mode == 0 )
    {
      circ_node_t *node[4];

      for(int i = 0; i<GET_ELEMENTS(node); i++)
        {
          if( i == 0 || i == 1) node[i] = circ_node_create( circuit, true );
          else node[i] = circ_node_create( circuit, false );

          ds_test_check( (node[i] == NULL), "circ_node_create()" );
          circuit_attach_node( circuit, node[i] );
        }

      ds_test_check( circ_element_config(( ADC1, ELM_CONFIG_SET, /*inc*/0, 2 )), "circ_element_config()" );

      circ_pin_set_node( VS1->pin_vector[0], node[0] );
      circ_pin_set_node( VS2->pin_vector[0], node[1] );
      circ_pin_set_node( ADC1->pin_vector[0], node[0] );
      circ_pin_set_node( ADC1->pin_vector[1], node[1] );
      circ_pin_set_node( ADC1->pin_vector[2], node[2] );
      circ_pin_set_node( ADC1->pin_vector[3], node[3] );
    }
  else if( mode == 1)
    {
      circ_node_t *node0 = circ_node_create( circuit, true );
      circuit_attach_node( circuit, node0 );

      circ_pin_set_node( VS1->pin_vector[0], node0 );
      circ_pin_set_node( ADC1->pin_vector[0], node0 );
    }

  ds_test_check( circ_element_config(( VS1, ELM_CONFIG_SET, /*Vh*/0, 5.0f )), "circ_element_config()" );
  ds_test_check( circ_element_config(( VS1, ELM_CONFIG_SET, /*g*/2, (int)true )), "circ_element_config()" );

  ds_test_check( circuit_init( circuit ), "circuit_init()" );

  ds_test_check( circuit_run_step( circuit ), "circuit_run_step()" );

  if( mode == 0 )
    {
      ds_test_check_float( circ_pin_get_volt( ADC1->pin_vector[0] ), 5.0f, error, "ADC: circ_pin_get_volt()" );
      ds_test_check_float( circ_pin_get_volt( ADC1->pin_vector[1] ), .0f, error, "ADC: circ_pin_get_volt()" );

      ds_test_check( circ_pin_get_state( ADC1->pin_vector[2] )!=SIG_HIGH, "ADC: circ_pin_get_logic_state()" );
      ds_test_check( circ_pin_get_state( ADC1->pin_vector[3] )!=SIG_LOW, "ADC: circ_pin_get_logic_state()" );

      ds_test_check( circ_element_config(( VS1, ELM_CONFIG_SET, /*g*/2, (int)false )), "circ_element_config()" );

      ds_test_check( circuit_run_step( circuit ), "circuit_run_step()" );

      ds_test_check_float( circ_pin_get_volt( ADC1->pin_vector[0] ), .0f, error, "ADC: circ_pin_get_volt()" );

      ds_test_check( circ_pin_get_state( ADC1->pin_vector[2] )!=SIG_LOW, "ADC: circ_pin_get_logic_state()" );
    }
  else if( mode == 1 )
    {
      ds_test_check_float( circ_pin_get_volt( ADC1->pin_vector[0] ), 5.0f, error, "ADC: circ_pin_get_volt()" );

      ds_test_check( circ_pin_get_state( ADC1->pin_vector[1] )!=SIG_FLOAT, "ADC: circ_pin_get_logic_state()" );
    }

  circuit_free( circuit );

  if( mode == 0 )
    {
      mode = 1;
      goto build_circuit;
    }

  matrix_free( matrix );

  return 0;
}
