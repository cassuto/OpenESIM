/** @file
 * Circuit framework
 */

/*
 *  OpenDSIM (A/D mixed circuit simulator)
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

#define TRACE_UNIT "circ-frame"

#include <dsim/barrier.h>
#include <dsim/logtrace.h>
#include <dsim/error.h>
#include <dsim/list.h>
#include <dsim/hashmap.h>
#include <dsim/node-template.h>
#include <dsim/string.h>
#include <dsim/circmatrix.h>
#include <model/model.h>
#include <model/circ-node.h>
#include <model/circ-element.h>

#include <math.h>

#include <dsim/circuit.h>


circuit_t *
circuit_create( circ_matrix_t *matrix )
{
  circuit_t *circuit = (circuit_t *)ds_heap_alloc(sizeof(*circuit));

  if ( circuit )
    {
      memset( circuit, 0, sizeof(*circuit) );
      circuit->circmatrix = matrix;
      list_init( &circuit->analog_element_list );
      list_init( &circuit->digital_element_list );
      list_init( &circuit->analog_node_list );
      list_init( &circuit->digital_node_list );

      if( hashmap_init( &circuit->changed_node_list, HASHMAP_KEY_INTPTR, 0 ) )
        goto err_out;
      if( hashmap_init( &circuit->changed_fast_list, HASHMAP_KEY_INTPTR, 0 ) )
        goto err_out;
      if( hashmap_init( &circuit->react_list, HASHMAP_KEY_INTPTR, 0 ) )
        goto err_out;
      if( hashmap_init( &circuit->non_linear_list, HASHMAP_KEY_INTPTR, 0 ) )
        goto err_out;
      if( hashmap_init( &circuit->logic_list_ping, HASHMAP_KEY_INTPTR, 0 ) )
        goto err_out;
      if( hashmap_init( &circuit->logic_list_pong, HASHMAP_KEY_INTPTR, 0 ) )
        goto err_out;

      circuit->rate = 1e6;
      circuit->fps = 20;
      circuit->steps_react = 50;
      circuit->steps_non_linear = 10;
    }
  return circuit;
err_out:
  if ( circuit ) ds_heap_free( circuit );
  return NULL;
}

int
circuit_init( circuit_t *circuit )
{
  int rc = 0;

  hashmap_clear( &circuit->changed_node_list, NULL /*collect*/ );
  hashmap_clear( &circuit->changed_fast_list, NULL /*collect*/ );
  hashmap_clear( &circuit->react_list, NULL /*collect*/ );
  hashmap_clear( &circuit->non_linear_list, NULL /*collect*/ );

  if( (rc = matrix_init( circuit_matrix(circuit), &circuit->analog_node_list )) )
    return rc;

  /* Initialize elements */
  foreach_list( circ_element_t, element, &circuit->analog_element_list )
    if( (rc = asim_element_init( element )) )
      return rc;

  foreach_list( circ_element_t, element, &circuit->digital_element_list )
    if( (rc = dsim_element_init( element )) )
      return rc;

  /* Initialize matrix */
  foreach_list( circ_element_t, element, &circuit->analog_element_list )
    if( (rc = asim_element_stamp( element )) )
      return rc;

  foreach_list( circ_node_t, node, &circuit->analog_node_list )
    if( (rc = circ_node_stamp_matrix( node )) )
      return rc;

  /* Set up initial parameters of each node */
  if( (rc = matrix_solve( circuit->circmatrix )) )
    return -DS_SOLVE_MATRIX;

  circuit->step = 0l;
  circuit->react_count = 0;
  circuit->non_linear_count = 0;
  circuit->non_linear_acc_index = 5;

  circuit_set_rate( circuit, circuit->fps, circuit->rate );

  return rc;
}

double
circuit_timestep( circuit_t *circuit )
{
  return circuit->timestep;
}

void
circuit_set_non_linear_acc_index( circuit_t *circuit, int acc_index )
{
  if     ( acc_index < 3 )  acc_index = 3;
  else if( acc_index > 14 ) acc_index = 14;
  circuit->non_linear_acc_index = acc_index;
}

double
circuit_non_linear_accuracy( circuit_t *circuit )
{
  return 1/pow(10,circuit->non_linear_acc_index)/2;
}

static int
solve_matrix( circuit_t *circuit )
{
  int rc = 0;

  foreach_hashmap( list_voidptr_node_t, node, &circuit->changed_node_list )
    if( (rc = circ_node_stamp_matrix( node->val )) )
      return rc;

  hashmap_clear( &circuit->changed_node_list, NULL/*collect*/ );

  return matrix_solve(circuit->circmatrix);
}

int
circuit_run_step( circuit_t *circuit )
{
  int rc = 0;
  hashmap_t *logic_list;

  for( int i=0; i < circuit->steprate; i++ )
    {
      circuit->step ++;

      /* Run Reactive Elements */
      if( ++(circuit->react_count) == circuit->steps_react )
        {
          circuit->react_count = 0;

          foreach_hashmap( list_voidptr_node_t, element, &circuit->react_list )
            if( (rc = asim_element_vchanged( (circ_element_t*)(element->val) )) )
              return rc;

          hashmap_clear( &circuit->react_list, NULL /*collect*/ );
        }

      /* Run changed-fast analogous elements */
      foreach_hashmap( list_voidptr_node_t, element, &circuit->changed_fast_list )
        if( (rc = asim_element_vchanged( (circ_element_t*)(element->val) )) )
          return rc;

      hashmap_clear( &circuit->changed_fast_list, NULL /*collect*/ );

      /* Run logic elements */

      if( circuit->logic_list_curr_ping ) /* ping */
        {
          logic_list = &circuit->logic_list_ping;
        }
      else /* pong */
        {
          logic_list = &circuit->logic_list_pong;
        }
      circuit->logic_list_curr_ping = !circuit->logic_list_curr_ping;

      foreach_list( circ_element_t, element, &circuit->analog_element_list )
        if( element->desc->mdel_type == MDEL_AD )
          if( (rc = dsim_element_clock( element )) ) /* AD model clk */
            return rc;

      foreach_list( circ_element_t, element, &circuit->digital_element_list )
        if( (rc = dsim_element_clock( element )) ) /* digital model clk */
          return rc;

      foreach_hashmap( list_voidptr_node_t, element, logic_list )
        if( (rc = dsim_element_event( (circ_element_t*)(element->val) )) )
          return rc;

      hashmap_clear( logic_list, NULL /*collect*/ );

      /* Run Non-Linear elements */
      if( ++(circuit->non_linear_count) == circuit->steps_non_linear )
        {
          circuit->non_linear_count = 0;
          int counter = 0;
          /*
           * Solve non-linear circuit until all converged
           */
          while( !hashmap_empty( &circuit->non_linear_list ) )
            {
              foreach_hashmap( list_voidptr_node_t, element, &circuit->non_linear_list )
                if( (rc = asim_element_vchanged( (circ_element_t*)(element->val) )) )
                  return rc;

              hashmap_clear( &circuit->non_linear_list, NULL /*collect*/ );

              if( !hashmap_empty(&circuit->changed_node_list) )
                if( (rc = solve_matrix( circuit )) )
                  return rc;

              if( ++counter > 1000 ) break; /* Limit the number of loops */
            }
          if( counter > 0 ) trace_debug(("circuit_run_step()  Non-Linear Solved in steps %d\n", counter));
          /* todo: point out whether converged and throw warring if no. */
        }
      if( !hashmap_empty(&circuit->changed_node_list) )
        if( (rc = solve_matrix( circuit )) )
          return rc;
    }
  return rc;
}

int
circuit_stop( circuit_t *circuit )
{
  int rc = 0;
  foreach_list( circ_node_t, node, &circuit->analog_node_list  ) circ_node_set_volt( node, .0f );
  foreach_list( circ_node_t, node, &circuit->digital_node_list  ) circ_node_set_state( node, SIG_LOW );

  foreach_list( circ_element_t, element, &circuit->analog_element_list )
    {
      if( (rc = asim_element_init( element )) )
        return rc;
      asim_element_reset( element );
    }
  foreach_list( circ_element_t, element, &circuit->digital_element_list )
    {
      if( (rc = dsim_element_init( element )) )
        return rc;
      dsim_element_reset( element );
    }

  return rc;
}

int
circuit_set_rate( circuit_t *circuit, int fps, int rate )
{
  if( rate > 1e6 ) rate = 1e6;
  if( rate < 1 )   rate = 1;

  circuit->steprate = rate/fps;
  circuit->fps = fps;

  if( rate < fps )
    {
      return -DS_FAULT;
    }
  circuit->rate = circuit->steprate*fps;
  return 0;
}

int
circuit_react_clock( circuit_t *circuit )
{
  return circuit->steps_react;
}

void
circuit_set_react_clock( circuit_t *circuit, int value )
{
  if     ( value < 1  )  value = 1;
  else if( value > 100 ) value = 100;

  circuit->steps_react = value;
}

int
circuit_non_linear_clock( circuit_t *circuit )
{
  return circuit->steps_non_linear;
}

void
circuit_set_non_linear_clock( circuit_t *circuit, int value )
{
  if     ( value < 1  ) value = 1;
  else if( value > 100 ) value = 100;

  circuit->steps_non_linear = value;
}

void
circuit_attach_element( circuit_t *circuit, circ_element_t *element )
{
  switch( element->desc->mdel_type )
  {
    case MDEL_AD:
    case MDEL_ANALOG:
      if( !list_contains( &circuit->analog_element_list, list_node(element) ) )
        {
          list_insert( &circuit->analog_element_list, list_node(element) );
        }
      break;

    case MDEL_DIGITAL:
      if( !list_contains( &circuit->digital_element_list, list_node(element) ) )
        {
          list_insert( &circuit->digital_element_list, list_node(element) );
        }
      break;
  }
}

void
circuit_attach_node( circuit_t *circuit, circ_node_t *node )
{
  list_t *node_list = node->analog ? &circuit->analog_node_list : &circuit->digital_node_list;

  if( !list_contains( node_list, list_node(node) ) )
    {
      node->circuit = circuit;
      list_insert( node_list, list_node(node) );
    }
}

int
circuit_add_changed_node( circuit_t *circuit, circ_node_t* node )
{
  if( !hashmap_contains_voidptr( &circuit->changed_node_list, node ) )
    {
      hashmap_new_at( &circuit->changed_node_list, (hashmap_key_t)node, hashmap_voidptr_node_t )->val = node;
    }
  return 0;
}

int
circuit_add_changed_fast( circuit_t *circuit, circ_element_t* element )
{
  if( !hashmap_contains_voidptr( &circuit->changed_fast_list, element ) )
    {
      hashmap_new_at( &circuit->changed_fast_list, (hashmap_key_t)element, hashmap_voidptr_node_t )->val = element;
    }
  return 0;
}

int
circuit_add_react( circuit_t *circuit, circ_element_t* element )
{
  if( !hashmap_contains_voidptr( &circuit->react_list, element ) )
    {
      hashmap_new_at( &circuit->react_list, (hashmap_key_t)element, hashmap_voidptr_node_t )->val = element;
    }
  return 0;
}

int
circuit_add_non_linear( circuit_t *circuit, circ_element_t* element )
{
  if( !hashmap_contains_voidptr( &circuit->non_linear_list, element ) )
    {
      hashmap_new_at( &circuit->non_linear_list, (hashmap_key_t)element, hashmap_voidptr_node_t )->val = element;
    }
  return 0;
}

int
circuit_add_logic( circuit_t *circuit, circ_element_t* element )
{
  /* Reference to ping-pong buffer */

  if( circuit->logic_list_curr_ping ) /* ping */
    {
      if( !hashmap_contains_voidptr( &circuit->logic_list_ping, element ) )
        {
          hashmap_new_at( &circuit->logic_list_ping, (hashmap_key_t)element, hashmap_voidptr_node_t )->val = element;
        }
    }
  else /* pong */
    {
      if( !hashmap_contains_voidptr( &circuit->logic_list_pong, element ) )
        {
          hashmap_new_at( &circuit->logic_list_pong, (hashmap_key_t)element, hashmap_voidptr_node_t )->val = element;
        }
    }
  return 0;
}

void
circuit_free( circuit_t *circuit )
{
  if( circuit )
    {
      list_release( &circuit->analog_node_list, circ_node_free );
      list_release( &circuit->digital_node_list, circ_node_free );
      list_release( &circuit->analog_element_list, circ_element_free );
      list_release( &circuit->digital_element_list, circ_element_free );

      hashmap_release( &circuit->changed_node_list, ds_heap_free );
      hashmap_release( &circuit->changed_fast_list, ds_heap_free );
      hashmap_release( &circuit->react_list, ds_heap_free );
      hashmap_release( &circuit->non_linear_list, ds_heap_free );
      hashmap_release( &circuit->logic_list_ping, ds_heap_free );
      hashmap_release( &circuit->logic_list_pong, ds_heap_free );
    }
  ds_heap_free( circuit );
}
