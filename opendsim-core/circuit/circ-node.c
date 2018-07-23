/** @file
 * Circuit node
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
#define TRACE_UNIT "circ-node"

#include <dsim/error.h>
#include <dsim/list.h>
#include <dsim/hashmap.h>
#include <dsim/node-template.h>
#include <dsim/logtrace.h>
#include <dsim/string.h>
#include <dsim/circuit.h>
#include <dsim/circmatrix.h>
#include <model/model.h>

#include <model/circ-node.h>

circ_node_t*
circ_node_create( circuit_t *circuit, bool analog )
{
  circ_node_t *node = (circ_node_t *)ds_heap_alloc( sizeof(*node) );
  if ( node )
    {
      memset( node, 0, sizeof(*node) );

      if( hashmap_init( &node->admit_list, HASHMAP_KEY_INTPTR, 0 ) ) /* <circ_pin_t*,double> */
        goto err_out;
      if( hashmap_init( &node->curr_list, HASHMAP_KEY_INTPTR, 0 ) ) /* <circ_pin_t*,double> */
        goto err_out;
      if( hashmap_init( &node->node_list, HASHMAP_KEY_INTPTR, 0 ) ) /* <circ_pin_t*,int> */
        goto err_out;
      if( hashmap_init( &node->admit, HASHMAP_KEY_INTPTR, 0 ) ) /* <int, double>) */
        goto err_out;

      list_init( &node->pin_list );

      list_init( &node->changed_fast_list );
      list_init( &node->react_list );
      list_init( &node->non_linear_list );
      list_init( &node->logic_list );

      node->analog = analog;
      node->circuit = circuit;
    }
  return node;

err_out:
  if ( node ) ds_heap_free( node );
  return NULL;
}

void
circ_node_init( circ_node_t *node )
{
  node->single  = false;
  node->changed = false;
  node->admit_changed = false;
  node->curr_changed = false;
  node->need_fast_update = false;
  node->volt = .0f;

  list_clear( &node->changed_fast_list, NULL /*collect*/ );
  list_clear( &node->react_list, NULL /*collect*/ );
  list_clear( &node->non_linear_list, NULL /*collect*/ );
  list_clear( &node->logic_list, NULL /*collect*/ );

  hashmap_clear( &node->admit_list, NULL /*collect*/ );
  hashmap_clear( &node->curr_list, NULL /*collect*/ );
  hashmap_clear( &node->node_list, NULL /*collect*/ );
  hashmap_clear( &node->admit, NULL /*collect*/ );
}

int
circ_node_stamp_current( circ_node_t * node, circ_pin_t* pin, double data )
{
  trace_assert( node->analog );
  hashmap_new_at( &(node->curr_list), (hashmap_key_t)pin, hashmap_double_node_t)->val = data;

  node->curr_changed = true;

  if( !node->changed )
    {
      node->changed = true;
      return circuit_add_changed_node( node->circuit, node );
    }
  return 0;
}

int
circ_node_stamp_admit( circ_node_t * node, circ_pin_t* pin, double data )
{
  trace_assert( node->analog );
  hashmap_new_at( &node->admit_list, (hashmap_key_t)pin, hashmap_double_node_t)->val = data;

  node->admit_changed = true;

  if( !node->changed )
    {
      node->changed = true;
      return circuit_add_changed_node( node->circuit, node );
    }
  return 0;
}

void
circ_node_set_index( circ_node_t *node, int n )
{
  node->node_index = n;
}

int
circ_node_stamp_matrix( circ_node_t * node )
{
  trace_assert( node->analog );
  node->changed = false;

  if( node->admit_changed )
    {
      double totalAdmit = 0;
      double adm;
      int    enode;
      hashmap_key_t pin;

      foreach_hashmap( hashmap_double_node_t, i, &node->admit )
        {
          i->val = .0f;
        }

      foreach_hashmap( hashmap_double_node_t, i, &node->admit_list )
        {
          adm = i->val;
          if ( !node->single )
            {
              pin = hashmap_node(i)->key;
              enode = hashmap_cast_at( &node->node_list, pin, hashmap_int_node_t)->val;

              /* add admit to the appropriate node */
              hashmap_new_at( &node->admit, enode, hashmap_double_node_t)->val += adm;
            }
          totalAdmit += adm;
        }

      if( node->single ) node->total_admit = totalAdmit; /* single element in matrix */
      else
        {
          foreach_hashmap( hashmap_double_node_t, i, &node->admit )
            {
              enode = hashmap_node(i)->key;
              if( enode > 0 )
                matrix_stamp( circuit_matrix(node->circuit), node->node_index, enode, -(i->val) );
            }

          matrix_stamp( circuit_matrix(node->circuit), node->node_index, node->node_index, totalAdmit );
        }
      node->admit_changed = false;
    }

  if( node->curr_changed )
    {
      double totalCurr  = 0;
      foreach_hashmap( hashmap_double_node_t, current, &node->curr_list ) totalCurr  += current->val;

      if( node->single )              /* single element in matrix */
        {
          if( node->total_admit > 0 ) node->volt = totalCurr/node->total_admit;
          circ_node_set_volt( node, node->volt );
        }
      else matrix_stamp_coef( circuit_matrix(node->circuit), node->node_index, totalCurr );

      node->curr_changed  = false;
    }
  return 0;
}

int
circ_node_set_volt( circ_node_t *node, double v )
{
  int rc = 0;
  trace_assert( node->analog );

  if( node->volt != v )
    {
      node->volt = v;

      foreach_list( list_voidptr_node_t, nd, &node->changed_fast_list )
        if( (rc = circuit_add_changed_fast( node->circuit, nd->val )) )
          return rc;

      foreach_list( list_voidptr_node_t, nd, &node->react_list )
        if( (rc = circuit_add_react( node->circuit, nd->val )) )
          return rc;

      foreach_list( list_voidptr_node_t, nd, &node->non_linear_list )
        if( (rc = circuit_add_non_linear( node->circuit, nd->val )) )
          return rc;
    }
  return 0;
}

int
circ_node_set_state( circ_node_t *node, logic_state_t state )
{
  int rc = 0;
  trace_assert( !node->analog );

  if( node->logic_state != state )
    {
      node->logic_state = state;

      foreach_list( list_voidptr_node_t, nd, &node->logic_list )
        if( (rc = circuit_add_logic( node->circuit, nd->val )) )
          return rc;
    }
  return 0;
}

int
circ_node_add_pin( circ_node_t *node, circ_pin_t *pin )
{
  if( !list_contains_voidptr( &node->pin_list, pin ) )
    {
      return list_insert_voidptr( &node->pin_list, pin );
    }
  return 0;
}

int
circ_node_remove_pin( circ_node_t *node, circ_pin_t *pin )
{
  foreach_list( list_voidptr_node_t, pinnode, &node->pin_list )
    {
      if( pinnode->val == pin )
        {
          list_remove( &node->pin_list, list_node(pinnode) );
          list_collect( &node->pin_list, pinnode );
          return 0;
        }
    }
  return -DS_FAULT;
}

int
circ_node_connect_pin( circ_node_t *node, circ_pin_t *pin, int node_comp_index )
{
  hashmap_new_at( &node->node_list, (hashmap_key_t)pin, hashmap_int_node_t)->val = node_comp_index;
  return 0;
}

int
circ_node_add_changed_fast( circ_node_t *node, circ_element_t* element )
{
  if( element && !list_contains_voidptr( &node->changed_fast_list, element ) )
    {
      return list_insert_voidptr( &node->changed_fast_list, element );
    }
  return 0;
}

int
circ_node_add_react( circ_node_t *node, circ_element_t* element )
{
  if( element && !list_contains_voidptr( &node->react_list, element ) )
    {
      return list_insert_voidptr( &node->react_list, element );
    }
  return 0;
}

int
circ_node_add_non_linear( circ_node_t *node, circ_element_t* element )
{
  if( element && !list_contains_voidptr( &node->non_linear_list, element ) )
    {
      return list_insert_voidptr( &node->non_linear_list, element );
    }
  return 0;
}

int
circ_node_add_logic( circ_node_t *node, circ_element_t* element )
{
  if( element && !list_contains_voidptr( &node->logic_list, element ) )
    {
      return list_insert_voidptr( &node->logic_list, element );
    }
  return 0;
}

void
circ_node_free( void *ptr )
{
  circ_node_t *node = (circ_node_t *)ptr;
  if( node )
    {
      list_release( &node->pin_list, ds_heap_free );
      list_release( &node->changed_fast_list, ds_heap_free );
      list_release( &node->react_list, ds_heap_free );
      list_release( &node->non_linear_list, ds_heap_free );
      list_release( &node->logic_list, ds_heap_free );

      hashmap_release( &node->admit_list, ds_heap_free );
      hashmap_release( &node->curr_list, ds_heap_free );
      hashmap_release( &node->node_list, ds_heap_free );
      hashmap_release( &node->admit, ds_heap_free );
    }
  ds_heap_free( node );
}
