/** @file
 * Circuit node
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
#include <dsim/circuit.h>
#include <dsim/circmatrix.h>

#include <model/circ-node.h>

////////////////////////////////////////////////////////////////////////////////

circ_node_t*
circ_node_create( circuit_t *circuit )
{
  circ_node_t *node = (circ_node_t *)ds_heap_alloc( sizeof(*node) );
  if ( node )
    {
      node->volt = .0f;
      node->node_index = 0;
      node->num_cons = 0;

      node->total_admit = .0f;

      node->single  = false;
      node->changed = false;
      node->admit_changed = false;
      node->curr_changed = false;
      node->need_fast_update = false;

      list_init( &node->pin_list );

      if( hashmap_init( &node->admit_list, HASHMAP_KEY_INTPTR, 16 ) ) /* <circ_pin_t*,double> */
        goto err_out;
      if( hashmap_init( &node->curr_list, HASHMAP_KEY_INTPTR, 16 ) ) /* <circ_pin_t*,double> */
        goto err_out;
      if( hashmap_init( &node->node_list, HASHMAP_KEY_INTPTR, 16 ) ) /* <circ_pin_t*,int> */
        goto err_out;
      if( hashmap_init( &node->admit, HASHMAP_KEY_INTPTR, 16 ) ) /* <int, double>) */
        goto err_out;

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

  list_clear( &node->pin_list, NULL );

  hashmap_clear( &node->admit_list, ds_heap_free );
  hashmap_clear( &node->curr_list, ds_heap_free );
  hashmap_clear( &node->node_list, ds_heap_free );
  hashmap_clear( &node->admit, ds_heap_free );
}

void
circ_node_stamp_current( circ_node_t * node, circ_pin_t* pin, double data )
{
  hashmap_entry(hashmap_at( &node->curr_list, (hashmap_key_t)pin ), hashmap_double_node_t)->val = data;

  node->curr_changed = true;

  if( !node->changed )
    {
      node->changed = true;
      circuit_add_changed_node( node->circuit, node );
    }
}

void
circ_node_stamp_admit( circ_node_t * node, circ_pin_t* pin, double data )
{
  hashmap_entry(hashmap_at( &node->admit_list, (hashmap_key_t)pin), hashmap_double_node_t)->val = data;

  node->admit_changed = true;

  if( !node->changed )
    {
      node->changed = true;
      circuit_add_changed_node( node->circuit, node );
    }
}

void
circ_node_set_index( circ_node_t *node, int n )
{
  node->node_index = n;
}

int
circ_node_stamp_matrix( circ_node_t * node )
{
  node->changed = false;

  if( node->admit_changed )
    {
      double totalAdmit = 0;
      double adm;
      int    enode;
      hashmap_key_t pin;
      hashmap_int_node_t *tab_node;

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
          double volt = 0;
          if( node->total_admit > 0 ) node->volt = totalCurr/node->total_admit;
          circ_node_set_volt( node, node->volt );
        }
      else matrix_stamp_coef( circuit_matrix(node->circuit), node->node_index, totalCurr );

      node->curr_changed  = false;
    }
  return 0;
}

void
circ_node_set_volt( circ_node_t *node, double v )
{
  if( node->volt != v )
    {
      node->volt = v;
    }
}

void
circ_node_free( circ_node_t *node )
{
  circ_node_init( node );
  ds_heap_free( node );
}
