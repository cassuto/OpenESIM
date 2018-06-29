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
#include <model/circ-node.h>
#include <model/circ-element.h>

#include "tsf/test.h"



int
main( int argc, char *argv[] )
{
  if ( ds_test_init( "test-circ-element-create" ) ) return 1;

  circ_matrix_t *matrix = matrix_create();

  ds_test_check( (matrix == NULL), "circ_matrix_create()" );

  circuit_t *circuit = circuit_create( matrix );

  ds_test_check( (circuit == NULL), "circuit_create()" );

  circ_element_t *element = circ_element_create( circuit, "C", 1 );

  ds_test_check( (element == NULL), "circ_element_create()" );

  circ_element_free( element );

  circuit_free( circuit );

  matrix_free( matrix );

  return 0;
}
