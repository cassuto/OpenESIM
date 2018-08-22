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

#include "d-buff.h"

int
LIB_FUNC(buff_event)( circ_element_t *element )
{
  int rc;
  int ports = circ_element_get_pin_count(element)/2;
  logic_state_t state;

  DEFINE_PARAM(param, element, buff_param_t);

  for( int i=0; i < ports; i++ )
    {
      state = GET_STATE( element->pin_vector[i] );
      if( state != param->state[i] )
        {
          param->state[i] = state;
          if( (rc = circ_pin_set_state( element->pin_vector[ports+i], state )) )
            return rc;
        }
    }
  return 0;
}
