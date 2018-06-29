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

#include "dac.h"

int
LIB_INT_FUNC(dac_set_vsrcs)( circ_element_t *element, dac_param_t *param, int n )
{
  if( param->vsrc_node )
    {
      for( int i=0; i < param->outputs_count; i++ )
        {
          circ_node_free( param->vsrc_node[i] );
        }
    }
  ds_heap_free( param->vsrc_node );

  param->vsrc_node = (circ_node_t **)ds_heap_alloc( n * sizeof(circ_node_t *) );

  if( param->vsrc_node )
    {
      for( int i=0; i < n; i++ )
        {
          param->vsrc_node[i] = circ_node_create( element->circuit, true );
          if( NULL==param->vsrc_node[i] )
            return -DS_NO_MEMORY;
          circ_node_set_index( param->vsrc_node[i], 0 );
        }

      param->outputs_count = n;
      return 0;
    }
  return -DS_NO_MEMORY;
}
