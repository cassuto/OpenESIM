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
LIB_FUNC(logicstate_event)( circ_element_t *element )
{
  logic_state_t state;
  int ports = circ_element_get_pin_count( element );
  DEFINE_PARAM(param, element, logicstate_param_t);

  for( int i=0; i<ports; i++ )
    {
      state = GET_STATE( element->pin_vector[i] );
      if( param->state[i] != state )
        {
          param->state[i] = state;
        }
    }

  return 0;
}
