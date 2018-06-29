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
LIB_FUNC(adc_vchanged)( circ_element_t *element )
{
  DEFINE_PARAM(param, element, adc_param_t);

  double volt;
  bool  state;
  int index;

  for( int i=0; i < param->inputs_count; i++)
    {
      /*
       * get voltage from analogous field and convert state
       */
      volt = GET_VOLT( element->pin_vector[i] );

      if( volt != param->volt[i] )
        {
          param->volt[i] = volt;

          state = param->state[i];
          if     ( volt > param->input_vh ) state = true;
          else if( volt < param->input_vl ) state = false;

          if( param->inverted[i] ) state = !state;
          param->state[i] = state;

          /*
           * send the state to logical field
           */
          return circ_pin_set_state( element->pin_vector[param->inputs_count + i], state ? SIG_HIGH : SIG_LOW );
        }
    }
  return 0;
}
