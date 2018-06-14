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

#ifndef CIRC_NODE_H_
#define CIRC_NODE_H_

#include <dsim/list.h>
#include <dsim/hashmap.h>
#include <dsim/types.h>

typedef struct circuit_s circuit_t;
typedef struct circ_pin_s circ_pin_t;

typedef struct circ_node_s
{
  DS_LIST_NODE();

  list_t pin_list;

  hashmap_t admit_list;
  hashmap_t curr_list;
  hashmap_t node_list;

  hashmap_t admit;
  double total_admit;

  double volt;
  int   node_index;
  int   num_cons;

  bool need_fast_update;
  bool curr_changed;
  bool admit_changed;
  bool changed;
  bool single;

  circuit_t *circuit;
} circ_node_t;

circ_node_t* circ_node_create( circuit_t *circuit );
void circ_node_init( circ_node_t *node );
void circ_node_stamp_current( circ_node_t * node, circ_pin_t* pin, double data );
void circ_node_stamp_admit( circ_node_t * node, circ_pin_t* pin, double data );
void circ_node_set_index( circ_node_t * node, int n );
int circ_node_stamp_matrix( circ_node_t * node );
void circ_node_set_volt( circ_node_t *node, double v );
void circ_node_free( circ_node_t *node );

/* true if this eNode can calculate it's own Volt */
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

#endif //!defined(CIRC_NODE_H_)
