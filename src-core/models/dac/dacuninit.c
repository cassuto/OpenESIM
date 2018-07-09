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

void
LIB_FUNC(dac_uninit)( circ_element_t *element )
{
  DEFINE_PARAM(param, element, dac_param_t);
  ds_heap_free( param->state );
  ds_heap_free( param->inverted );
  if( param->vsrc_node )
    {
      for( int i=0; i < param->outputs_count; i++ )
        {
          circ_node_free( param->vsrc_node[i] );
        }
    }
  ds_heap_free( param->vsrc_node );
}
