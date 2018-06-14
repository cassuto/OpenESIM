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
#ifndef DSIM_CIRCUIT_H_
#define DSIM_CIRCUIT_H_

#include <dsim/list.h>

typedef struct circuit_s
{
  list_t node_list;     /* node = circ_node_t */
  list_t element_list;  /* node = circ_element_t */

  list_t changed_node_list; /* voidptr_node */
  list_t changed_fast_list; /* voidptr_node */
  list_t reactive_list;     /* voidptr_node */
  list_t non_linear_list;   /* voidptr_node */

  struct circ_matrix_s *circmatrix;
} circuit_t;

#define circuit_matrix(circuit) (circuit->circmatrix)

typedef struct circ_node_s circ_node_t;

circuit_t *circuit_create( struct circ_matrix_s *matrix );
int circuit_init( circuit_t *circuit );
int circuit_add_changed_node( circuit_t *circuit, circ_node_t* node );
int circuit_add_changed_fast( circuit_t *circuit, circ_node_t* node );
int circuit_add_reactive( circuit_t *circuit, circ_node_t* node );
int circuit_add_non_linear( circuit_t *circuit, circ_node_t* node );

#endif //!defined(DSIM_CIRCUIT_H_)
