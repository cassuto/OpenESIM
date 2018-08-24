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
LIB_FUNC(dac_init)( circ_element_t *element )
{
  DEFINE_PARAM(param, element, dac_param_t);
  int rc, index;

  param->state = (bool *)ds_heap_alloc( param->outputs_count * sizeof(bool) );
  param->inverted = (bool *)ds_heap_alloc( param->outputs_count * sizeof(bool) );
  param->volt = (double *)ds_heap_alloc( param->outputs_count * sizeof(double) );

  if( param->state && param->inverted && param->volt )
    {
      memset( param->state, 0, param->outputs_count * sizeof(bool) );
      memset( param->inverted, 0, param->outputs_count * sizeof(bool) );
      memset( param->volt, 0, param->outputs_count * sizeof(double) );
    }
  else return -DS_NO_MEMORY;

  circ_element_set_digital_pin( element, 0, param->outputs_count-1 );

  for( int i=0; i < param->outputs_count; i++ )
    {
      /* digital field */
      if( element->pin_vector[i]->connected )
        if( (rc = circ_node_add_logic( PINNODE(element, i), element )) )
          return rc;

      /* analog field */
      index = param->outputs_count + i;

      if( (rc = circ_pin_set_nodecomp(element->pin_vector[index], param->vsrc_node[i])) )
        return rc;
    }

  return dac_stamp( element );
}
