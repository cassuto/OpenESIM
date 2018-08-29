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

#include "logicstate.h"

int
LIB_FUNC(logicstate_init)( circ_element_t *element )
{
  int rc;
  int ports = circ_element_get_pin_count(element);
  DEFINE_PARAM(param, element, logicstate_param_t);

  param->state = (logic_state_t*)ds_heap_alloc( ports * sizeof(logic_state_t) );

  if( param->state )
    {
      memset( param->state, 0, ports * sizeof(logic_state_t) );
      for( int i=0; i < ports; i++ )
        if( element->pin_vector[i]->connected )
          if( (rc = circ_node_add_logic( PINNODE(element, i), element )) )
            return rc;

      return 0;
    }
  return -DS_NO_MEMORY;
}
