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

#include "adc.h"

int
LIB_FUNC(adc_init)( circ_element_t *element )
{
  DEFINE_PARAM(param, element, adc_param_t);
  int rc, index;

  param->state = (bool *)ds_heap_alloc( param->inputs_count * sizeof(bool) );
  param->inverted = (bool *)ds_heap_alloc( param->inputs_count * sizeof(bool) );
  param->volt = (double *)ds_heap_alloc( param->inputs_count * sizeof(double) );

  if( param->state && param->inverted && param->volt )
    {
      memset( param->state, 0, param->inputs_count * sizeof(bool) );
      memset( param->inverted, 0, param->inputs_count * sizeof(bool) );
      memset( param->volt, 0, param->inputs_count * sizeof(double) );
    }
  else return -DS_NO_MEMORY;

  for( int i=0; i < param->inputs_count; i++ )
    {
      /* analog field */
      if( (rc = circ_node_add_changed_fast( PINNODE(element,i), element )) )
        return rc;

      if( (rc = circ_pin_set_nodecomp(element->pin_vector[i], param->vsrc_node)) )
        return rc;

      /* digital field */
      index = param->inputs_count + i;

      if( element->pin_vector[index]->connected )
        if( (rc = circ_node_add_logic( PINNODE(element, index), element )) )
          return rc;
    }

  return adc_stamp( element );
}
