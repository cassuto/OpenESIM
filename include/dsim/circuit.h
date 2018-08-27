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
#ifndef DSIM_CIRCUIT_H_
#define DSIM_CIRCUIT_H_

#include <dsim/cdecl.h>
#include <dsim/list.h>
#include <dsim/hashmap.h>

C_DECLS

typedef struct circuit_s
{
  list_t analog_node_list;      /* node = circ_node_t */
  list_t digital_node_list;     /* node = circ_node_t */
  list_t analog_element_list;   /* node = circ_element_t */
  list_t digital_element_list;  /* node = circ_element_t */

  hashmap_t changed_node_list; /* voidptr_node */
  hashmap_t changed_fast_list; /* voidptr_node */
  hashmap_t react_list;        /* voidptr_node */
  hashmap_t non_linear_list;   /* voidptr_node */
  hashmap_t logic_list_ping;   /* voidptr_node */
  hashmap_t logic_list_pong;   /* voidptr_node */
  bool logic_list_curr_ping;

  struct circ_matrix_s *circmatrix;

  double timestep;
  uint64_t step;

  int rate;
  int fps;
  int steprate;
  int steps_react;
  int react_count;
  int steps_non_linear;
  int non_linear_count;
  int non_linear_acc_index;

} circuit_t;

#define circuit_matrix(circuit) (circuit->circmatrix)

typedef struct circ_node_s circ_node_t;
typedef struct circ_element_s circ_element_t;
typedef int (*pfn_sync_clock)( void *opaque );

circuit_t *circuit_create( struct circ_matrix_s *matrix );
int circuit_init( circuit_t *circuit );
double circuit_timestep( circuit_t *circuit );
void circuit_set_non_linear_acc_index( circuit_t *circuit, int acc_index );
double circuit_non_linear_accuracy( circuit_t *circuit );
int circuit_run_step( circuit_t *circuit, pfn_sync_clock clk, void *opaque );
int circuit_fast_update( circuit_t *circuit, const circ_node_t *node );
int circuit_stop( circuit_t *circuit );
int circuit_set_rate( circuit_t *circuit, int fps, int rate );
int circuit_react_clock( circuit_t *circuit );
void circuit_set_react_clock( circuit_t *circuit, int value );
int circuit_non_linear_clock( circuit_t *circuit );
void circuit_set_non_linear_clock( circuit_t *circuit, int value );
void circuit_attach_element( circuit_t *circuit, circ_element_t *element );
void circuit_attach_node( circuit_t *circuit, circ_node_t *node );
int circuit_add_changed_node( circuit_t *circuit, circ_node_t* node );
int circuit_add_changed_fast( circuit_t *circuit, circ_element_t* element );
int circuit_add_react( circuit_t *circuit, circ_element_t* element );
int circuit_add_non_linear( circuit_t *circuit, circ_element_t* element );
int circuit_add_logic( circuit_t *circuit, circ_element_t* element );
void circuit_free( circuit_t *circuit );

END_C_DECLS

#endif //!defined(DSIM_CIRCUIT_H_)
