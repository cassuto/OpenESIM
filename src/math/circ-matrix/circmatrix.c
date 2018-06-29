/** @file
 * Circ matrix
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

#include <math.h>

#define TRACE_UNIT "circmatrix"

#include <dsim/memory.h>
#include <dsim/error.h>
#include <dsim/logtrace.h>
#include <dsim/list.h>
#include <dsim/hashmap.h>
#include <dsim/node-template.h>
#include <dsim/string.h>

#include <dsim/circmatrix.h>

////////////////////////////////////////////////////////////////////////////////

circ_matrix_t *
matrix_create( void )
{
  circ_matrix_t *matrix = (circ_matrix_t *)ds_heap_alloc( sizeof(*matrix) );
  if ( matrix )
    {
      memset( matrix, 0, sizeof(*matrix) );
    }
  return matrix;
}

static void
free_vectors( circ_matrix_t *matrix )
{
  if ( matrix->circ_matrix )
    {
      ds_heap_free( matrix->circ_matrix[0] );
      ds_heap_free( matrix->circ_matrix );
      matrix->circ_matrix = NULL;
    }
  if ( matrix->a )
    {
      ds_heap_free( matrix->a[0] );
      ds_heap_free( matrix->a );
      matrix->a = NULL;
    }
  if( matrix->node_volt ) ds_heap_free( matrix->node_volt );
  if( matrix->coef_vector ) ds_heap_free( matrix->coef_vector );
  if( matrix->b ) ds_heap_free( matrix->b );
  if( matrix->ipvt ) ds_heap_free( matrix->ipvt );
  matrix->node_volt =
  matrix->coef_vector =
  matrix->b = NULL;
  matrix->ipvt = NULL;
}

void
matrix_free( circ_matrix_t *matrix )
{
  if ( matrix )
    {
      free_vectors( matrix );
    }
  ds_heap_free( matrix );
}

static double **
matrix_alloc( int rows, int cols )
{
  double **vect = (double **)ds_heap_alloc( cols * sizeof(double *) );
  if ( vect )
    {
      vect[0] = (double *)ds_heap_alloc( rows * cols * sizeof(double) );
      if ( vect[0] )
        for(int i=1; i<cols; i++)
          vect[i] = vect[i-1] + rows;
      else
        { ds_heap_free( vect ); vect = NULL; }
    }
  return vect;
}

/* After called matrix_init(), 'node_list' and 'element_list' should NOT be changed until the matrix is out of use */
int
matrix_init( circ_matrix_t *matrix, const list_t *node_list )
{
  int i = 0;

  matrix->inited = false;
  matrix->node_list = node_list;
  matrix->num_nodes = list_size( matrix->node_list );

  if( !matrix->num_nodes ) return -DS_FAULT;

  /*
   * Reallocate memory for matrix and vectors
   */
  free_vectors( matrix );
  matrix->circ_matrix = matrix_alloc( matrix->num_nodes, matrix->num_nodes );

  matrix->node_volt = (double *)ds_heap_alloc( sizeof(double) * matrix->num_nodes );
  matrix->coef_vector = (double *)ds_heap_alloc( sizeof(double) * matrix->num_nodes );

  matrix->a = matrix_alloc( matrix->num_nodes, matrix->num_nodes );
  matrix->b = (double *)ds_heap_alloc( sizeof(double) * matrix->num_nodes );
  matrix->ipvt = (int *)ds_heap_alloc( sizeof(int) * matrix->num_nodes );

  matrix->circ_matrix_size = sizeof(double) * matrix->num_nodes * matrix->num_nodes;
  matrix->coef_vector_size = sizeof(double) * matrix->num_nodes;

  if ( !matrix->circ_matrix || !matrix->node_volt || !matrix->coef_vector || !matrix->a || !matrix->b || !matrix->ipvt )
    {
      return -DS_NO_MEMORY;
    }

  /*
   * Initialize Matrixs & Vectors
   */
  for( i=0; i < matrix->num_nodes; i++ )
    {
      for( int j=0; j < matrix->num_nodes; j++ )
        {
          matrix->circ_matrix[i][j] = 0;
          matrix->a[i][j] = 0;
        }
      matrix->coef_vector[i] = 0;
      matrix->node_volt[i] = 0;
      matrix->ipvt[i] = 0;
      matrix->b[i] = 0;
    }

  matrix->admit_changed = false;
  matrix->curr_changed  = false;

  /*
   * Number and Initialize Nodes
   */
  i = 0;
  foreach_list( circ_node_t, node, matrix->node_list )
    {
      if ( !node->single ) /* skip the single element */
        {
          circ_node_set_index( node, i+1 );
          circ_node_init( node );
          i++;
        }
    }
  matrix->inited = true;
  return 0;
}

void
matrix_stamp( circ_matrix_t *matrix, int row, int col, double value )
{
  if( ( row == 0 )|( col == 0 ) ) return;

  trace_assert( matrix->inited );
  trace_debug(("matrix_stamp() row: %d, col: %d, value: %f\n", row, col, value));

  matrix->admit_changed = true;
  matrix->circ_matrix[row-1][col-1] = value;  /* circ_node numbers start at 1 */
}

void
matrix_stamp_coef( circ_matrix_t *matrix, int row, double value )
{
  if( row == 0 ) return;

  trace_assert( matrix->inited );
  trace_debug(("matrix_stamp_coef() row: %d, value: %f\n", row, value));

  matrix->curr_changed = true;
  matrix->coef_vector[row-1] = value;
}

int
matrix_solve( circ_matrix_t *matrix )
{
  int rc = 0;
  if( !matrix->admit_changed && !matrix->curr_changed ) return 0;

  int n = matrix->num_nodes;

  if( matrix->admit_changed )      /* Only factor admitance matrix if changed */
    {
      /* factors a matrix into upper and lower triangular matrices by
       * gaussian elimination.  On entry, a[0..n-1][0..n-1] is the
       * matrix to be factored.  ipvt[] returns an integer vector of pivot
       * indices, used in the lu_solve() routine. */

      memcpy( matrix->a[0], matrix->circ_matrix[0], matrix->circ_matrix_size );  /* Copy matrix */

      double scaleFactors[n];
      int i,j,k;

      /* divide each row by its largest element, keeping track of the scaling factors */
      for( i=0; i<n; i++ )
        {
          double largest = 0;
          for( j=0; j<n; j++ )
            {
              double x = fabs( matrix->a[i][j] );
              if( x > largest ) largest = x;
            }
          /* if all zeros, it's a singular matrix */
          if( largest == 0 ) return -DS_FAULT;
          scaleFactors[i] = 1.0/largest;
        }

      /* use Crout's method; loop through the columns */
      for( j=0; j<n; j++ )
        {
          /* calculate upper triangular elements for this column */
          for( i=0; i<j; i++ )
            {
              double q = matrix->a[i][j];
              for( k=0; k<i; k++ ) q -= (matrix->a[i][k]) * (matrix->a[k][j]);

              matrix->a[i][j] = q;
            }

          /* calculate lower triangular elements for this column */
          double largest = 0;
          int largestRow = -1;
          for( i=j; i<n; i++ )
            {
              double q = matrix->a[i][j];
              for( k=0; k<j; k++ ) q -= (matrix->a[i][k]) * (matrix->a[k][j]);

              matrix->a[i][j] = q;
              double x = fabs( q );
              if( x >= largest )
                {
                  largest = x;
                  largestRow = i;
                }
            }

          if( j != largestRow ) /* pivoting */
            {
              double x;
              for( k=0; k<n; k++ )
                {
                  x = matrix->a[largestRow][k];
                  matrix->a[largestRow][k] = matrix->a[j][k];
                  matrix->a[j][k] = x;
                }
              scaleFactors[largestRow] = scaleFactors[j];
            }

          matrix->ipvt[j] = largestRow;  /* keep track of row interchanges */

          if( matrix->a[j][j] == 0.0 ) matrix->a[j][j]=1e-18; /* avoid zeros */

          if( j != n-1 )
            {
              double mult = 1.0/matrix->a[j][j];
              for( i=j+1; i<n; i++ ) matrix->a[i][j] *= mult;
            }
        }
    }

  /* Solves the set of n linear equations using a LU factorization
   * previously performed by solveMatrix.  On input, b[0..n-1] is the right
   * hand side of the equations, and on output, contains the solution. */

  memcpy( matrix->b, matrix->coef_vector, matrix->coef_vector_size ); /* Copy vector */

  int i;
  for( i=0; i<n; i++ )                 /* find the first nonzero b element */
    {
      int row = matrix->ipvt[i];

      double swap = matrix->b[row];
      matrix->b[row] = matrix->b[i];
      matrix->b[i] = swap;
      if( swap != 0 ) break;
    }

  int bi = i++;
  for( /*i = bi*/; i < n; i++ )
    {
      int row = matrix->ipvt[i];
      int j;
      double tot = matrix->b[row];

      matrix->b[row] = matrix->b[i];
      /* forward substitution using the lower triangular matrix */
      for( j=bi; j<i; j++ ) tot -= (matrix->a[i][j]) * (matrix->b[j]);

      matrix->b[i] = tot;
    }
  for( i=n-1; i>=0; i-- )
    {
      double tot = matrix->b[i];

      /* back-substitution using the upper triangular matrix */
      for( int j=i+1; j<n; j++ ) tot -= (matrix->a[i][j]) * (matrix->b[j]);

      matrix->b[i] = tot/matrix->a[i][i];
    }
  matrix->admit_changed = false;
  matrix->curr_changed  = false;

  /*
   * Update node volt, notifying all nodes
   */
  i = 0;
  double volt;
  foreach_list( circ_node_t, node, matrix->node_list )
    {
      volt = matrix->b[i];
      if( isnan( volt ) ) return -DS_FAULT;

      if( (rc = circ_node_set_volt( node, volt )) )
        return rc;
      i++;
    }
  return 0;
}

/* return > 0: the new size of matrix
 * return < 0: the error code */
int
matrix_simplify( circ_matrix_t *matrix )
{
  int rc;
  list_t single_els;

  list_init( &single_els );

  for( int y=0; y<matrix->num_nodes; y++ )              /* Find Single Element Rows */
    {
      int nonCero = 0;
      for( int x=0; x<matrix->num_nodes; x++ )          /* Find Cero Row */
        {
          if( matrix->circ_matrix[x][y] == 0 )continue;
          nonCero++;
        }
      if( nonCero < 2 )
        {
          if ( (rc = list_insert_int( &single_els, y )) )
            return rc;
        }
    }

  int y = 0;
  foreach_list( circ_node_t, node, matrix->node_list )
    {
      if ( list_contains_int( &single_els, y ) )
        {
          circ_node_set_single( node, true );
          continue;
        }

      y++;
    }
  trace_debug(( "\nmatrix_simplify()\n"));

  list_release( &single_els, ds_heap_free );
  return y;
}

void
matrix_print( circ_matrix_t *matrix )
{
  trace_info(("Dump Admitance Matrix:\n"));
  for( int i=0; i<matrix->num_nodes; i++ )
    {
      for( int j=0; j<matrix->num_nodes; j++ )
        {
          trace_info(("%f \t", matrix->circ_matrix[i][j]));
        }
      trace_info(("\t coef = %f\n", matrix->coef_vector[i]));
    }
}

