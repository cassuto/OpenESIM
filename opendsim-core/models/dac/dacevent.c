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
LIB_FUNC(dac_event)( circ_element_t *element )
{
  DEFINE_PARAM(param, element, dac_param_t);
  int outputs_count, index;
  int rc;
  double volt;
  logic_state_t state;

  outputs_count = param->outputs_count;

  for( int i=0; i < outputs_count; i++ )
    {
      /*
       * Get state from logic field and convert voltage
       */
      state = GET_STATE( element->pin_vector[i] );
      if( state != param->state[i] )
        {
          param->state[i] = state;

          switch( state )
          {
            case SIG_HIGH:
            case SIG_RISE:
              volt = param->inverted[i] ? param->output_vl : param->output_vh;
              break;

            case SIG_LOW:
            case SIG_FALL:
              volt = param->inverted[i] ? param->output_vh : param->output_vl;
              break;

            default:
              volt = param->volt[i];
          }

          param->volt[i] = volt;
          index = outputs_count + i;

          if( (rc = circ_node_set_volt( param->vsrc_node[i], volt )) )
            return rc;

          if( (rc = circ_node_stamp_current( PINNODE(element, index), element->pin_vector[index], volt/param->imp )) )
            return rc;

        }
    }
  return rc;
}
