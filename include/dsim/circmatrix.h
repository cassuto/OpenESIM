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
#ifndef CIRCMATRIX_H_
#define CIRCMATRIX_H_

#include <dsim/types.h>
#include <dsim/list.h>
#include <dsim/cdecl.h>

#include <model/circ-node.h>
#include <model/circ-element.h>

C_DECLS

typedef struct circ_matrix_s
{
  int num_nodes;

  const list_t *node_list;

  double **circ_matrix;
  double *coef_vector;
  double *node_volt;
  size_t circ_matrix_size;
  size_t coef_vector_size;

  double **a;
  double *b;
  int* ipvt;

  bool admit_changed;
  bool curr_changed;
  bool inited;
} circ_matrix_t;


circ_matrix_t *matrix_create( void );
void matrix_print( circ_matrix_t *matrix );
int matrix_init( circ_matrix_t *matrix, const list_t *node_list );
void matrix_stamp( circ_matrix_t *matrix, int row, int col, double value );
void matrix_stamp_coef( circ_matrix_t *matrix, int row, double value );
int matrix_simplify( circ_matrix_t *matrix );
int matrix_solve( circ_matrix_t *matrix );
void matrix_free( circ_matrix_t *matrix );

/* helper functions */
static inline double **
matrix_get( const circ_matrix_t *matrix ){ return matrix->circ_matrix; }
static inline double *
matrix_get_coeff_vect( const circ_matrix_t *matrix ){ return matrix->coef_vector; }

END_C_DECLS

#endif //!defined(CIRCMATRIX_H_)
