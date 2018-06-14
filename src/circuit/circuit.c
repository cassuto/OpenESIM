/** @file
 * Circuit framework
 */

/*
 *  OpenDSIM (Opensource Circuit Simulator)
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

#include <dsim/error.h>
#include <dsim/list.h>
#include <dsim/hashmap.h>
#include <dsim/node-template.h>
#include <dsim/circmatrix.h>

#include <dsim/circuit.h>

////////////////////////////////////////////////////////////////////////////////

circuit_t *
circuit_create( circ_matrix_t *matrix )
{
  circuit_t *circuit = (circuit_t *)ds_heap_alloc(sizeof(*circuit));

  if ( circuit )
    {
      circuit->circmatrix = matrix;
      list_init( &circuit->element_list );
      list_init( &circuit->node_list );

      list_init( &circuit->changed_node_list );
      list_init( &circuit->changed_fast_list );
      list_init( &circuit->reactive_list);
      list_init( &circuit->non_linear_list);
    }
  return circuit;
}

int
circuit_init( circuit_t *circuit )
{
  int rc = 0;

  if ( (rc = matrix_init( circuit_matrix(circuit), &circuit->node_list, &circuit->element_list )) )
    return rc;
#if 0 //!fixme
  foreach_list( circ_element_t, elem, &circuit->element_list )
    elem->initialize();
  foreach_list( circ_element_t, elem, &circuit->element_list )
    elem->stamp();
#endif
  foreach_list( circ_node_t, node, &circuit->node_list )
    circ_node_stamp_matrix( node );

  return rc;
}

int
circuit_add_changed_node( circuit_t *circuit, circ_node_t* node )
{
  if( !list_contains_voidptr( &circuit->changed_node_list, node ) )
    {
      return list_insert_voidptr( &circuit->changed_node_list, node );
    }
  return 0;
}

int
circuit_add_changed_fast( circuit_t *circuit, circ_node_t* node )
{
  if( !list_contains_voidptr( &circuit->changed_fast_list, node ) )
    {
      return list_insert_voidptr( &circuit->changed_fast_list, node );
    }
  return 0;
}

int
circuit_add_reactive( circuit_t *circuit, circ_node_t* node )
{
  if( !list_contains_voidptr( &circuit->reactive_list, node ) )
    {
      return list_insert_voidptr( &circuit->reactive_list, node );
    }
  return 0;
}

int
circuit_add_non_linear( circuit_t *circuit, circ_node_t* node )
{
  if( !list_contains_voidptr( &circuit->non_linear_list, node ) )
    {
      return list_insert_voidptr( &circuit->non_linear_list, node );
    }
  return 0;
}
