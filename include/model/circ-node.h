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

#ifndef CIRC_NODE_H_
#define CIRC_NODE_H_

#include <dsim/list.h>
#include <dsim/hashmap.h>
#include <dsim/types.h>
#include <model/logic_signal.h>
#include <dsim/cdecl.h>

C_DECLS

typedef struct circuit_s circuit_t;
typedef struct circ_pin_s circ_pin_t;

typedef struct circ_node_s
{
  DS_LIST_NODE();

  bool analog;
  double volt;
  logic_state_t logic_state;
  int   node_index;

  list_t pin_list; /* voidptr_node */

  hashmap_t admit_list; /* double_node */
  hashmap_t curr_list;  /* double_node */
  hashmap_t node_list;  /* int_node */

  hashmap_t admit;      /* double_node */
  double total_admit;

  bool need_fast_update;
  bool curr_changed;
  bool admit_changed;
  bool changed;
  bool single;

  list_t changed_fast_list; /* voidptr_node */
  list_t react_list;        /* voidptr_node */
  list_t non_linear_list;   /* voidptr_node */
  list_t logic_list; /* voidptr_node */

  circuit_t *circuit;
} circ_node_t;

typedef struct circ_element_s circ_element_t;

circ_node_t* circ_node_create( circuit_t *circuit, bool analog );
void circ_node_init( circ_node_t *node );
int circ_node_stamp_current( circ_node_t * node, circ_pin_t* pin, double data );
int circ_node_stamp_admit( circ_node_t * node, circ_pin_t* pin, double data );
void circ_node_set_index( circ_node_t * node, int n );
int circ_node_stamp_matrix( circ_node_t * node );
int circ_node_set_volt( circ_node_t *node, double v );
int circ_node_set_state( circ_node_t *node, logic_state_t state );
int circ_node_add_pin( circ_node_t *node, circ_pin_t *pin );
int circ_node_remove_pin( circ_node_t *node, circ_pin_t *pin );
int circ_node_connect_pin( circ_node_t *node, circ_pin_t * pin, int node_comp_index );
int circ_node_add_changed_fast( circ_node_t *node, circ_element_t* element );
int circ_node_add_react( circ_node_t *node, circ_element_t* element );
int circ_node_add_non_linear( circ_node_t *node, circ_element_t* element );
int circ_node_add_logic( circ_node_t *node, circ_element_t* element );
void circ_node_free( void *node );

static inline logic_state_t
circ_node_get_logic_state( circ_node_t *node )
{
  return node->logic_state;
}

/* true if this node can calculate it's own Volt */
static inline void
circ_node_set_single( circ_node_t *node, bool single )
{
  node->single = single;
}

static inline double
circ_node_get_volt( const circ_node_t *node )
{
  return node->volt;
}

static inline int
circ_node_get_index( const circ_node_t *node )
{
  return node->node_index;
}

END_C_DECLS

#endif //!defined(CIRC_NODE_H_)
