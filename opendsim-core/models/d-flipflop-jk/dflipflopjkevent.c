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

#include "d-flipflop-jk.h"

int
LIB_FUNC(flipflop_jk_event)( circ_element_t *element )
{
  DEFINE_PARAM(param, element, flipflop_jk_param_t);

  bool clk = SIG_HIGH == GET_STATE( element->pin_vector[2] );
  bool clk_rising = ( !param->clk &&  clk );
  param->clk = clk;

  bool set = SIG_LOW == GET_STATE( element->pin_vector[0] );
  bool reset = SIG_LOW == GET_STATE( element->pin_vector[1] );

  if( set && reset )
    {
      circ_pin_set_state( element->pin_vector[5], SIG_HIGH );
      circ_pin_set_state( element->pin_vector[6], SIG_HIGH );
    }
  if( set )          /* master set */
    {
      circ_pin_set_state( element->pin_vector[5], SIG_HIGH );
      circ_pin_set_state( element->pin_vector[6], SIG_LOW );
    }
  else if( reset )   /* master reset */
    {
      circ_pin_set_state( element->pin_vector[5], SIG_LOW );
      circ_pin_set_state( element->pin_vector[6], SIG_HIGH );
    }
  else if( clk_rising )
    {
      bool J = SIG_HIGH == GET_STATE( element->pin_vector[3] );
      bool K = SIG_HIGH == GET_STATE( element->pin_vector[4] );
      bool Q = SIG_HIGH == GET_STATE( element->pin_vector[5] );

      logic_state_t state = (J && !Q) || (!K && Q) ? SIG_HIGH : SIG_LOW;

      circ_pin_set_state( element->pin_vector[5], state );
      circ_pin_set_state( element->pin_vector[6], logic_reversed(state) );
    }

  return 0;
}
