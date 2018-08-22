/** @file
 * Circuit PIN
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

#define TRACE_UNIT "pin"

#include <dsim/logtrace.h>
#include <dsim/error.h>
#include <dsim/memory.h>
#include <model/circ-node.h>

#include <model/circ-pin.h>


circ_pin_t*
circ_pin_create( void )
{
  circ_pin_t *pin = (circ_pin_t *)ds_heap_alloc( sizeof(*pin) );
  if ( pin )
    {
      pin->connected = false;
      pin->index = 0;
      pin->node = pin->node_comp = NULL;
    }
  return pin;
}

int
circ_pin_set_node( circ_pin_t *pin, struct circ_node_s *node )
{
  int rc = 0;
  if( node == pin->node ) return rc;

  if( pin->node )
    rc = circ_node_remove_pin( pin->node, pin );
  trace_assert( !rc );

  if( node )
    rc = circ_node_add_pin( node, pin );

  if ( !rc )
    {
      pin->node = node;
      pin->connected = (NULL!=node);
    }
  return rc;
}

int
circ_pin_set_nodecomp( circ_pin_t *pin, struct circ_node_s *node )
{
  trace_assert( ((pin->node && pin->node->analog) || (!pin->node)) && (( node && node->analog) || (!node)) );
  pin->node_comp = node;
  int node_comp_index = 0;
  if( node ) node_comp_index = circ_node_get_index( node );
  if( pin->connected ) return circ_node_connect_pin( pin->node, pin, node_comp_index );
  return 0;
}

double
circ_pin_get_volt( circ_pin_t *pin )
{
  if( pin->connected ) return circ_node_get_volt( pin->node );
  if( pin->node_comp ) return circ_node_get_volt( pin->node_comp );
  return .0f;
}

logic_state_t
circ_pin_get_state( circ_pin_t *pin )
{
  if( pin->connected ) return circ_node_get_logic_state( pin->node );
  if( pin->node_comp ) return circ_node_get_logic_state( pin->node_comp );
  return SIG_FLOAT;
}

void
circ_pin_free( circ_pin_t *pin )
{
  ds_heap_free( pin );
}
