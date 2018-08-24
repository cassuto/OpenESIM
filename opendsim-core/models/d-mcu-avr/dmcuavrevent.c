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

#include "dmcuavr.h"

int
LIB_FUNC(mcu_avr_event)( circ_element_t *element )
{
  int ports = circ_element_get_pin_count(element);
  logic_state_t state;

  DEFINE_PARAM(param, element, mcu_avr_param_t);

  for( int i=0; i < ports; i++ )
    {
      if( param->mcu->pinmap[i].typemask & PIN_ADC )
        {
          if( circ_pin_get_volt( element->pin_vector[i] ) > param->vth )
            avr_raise_irq( param->wr_port_irqs[i], 1 );
          else
            avr_raise_irq( param->wr_port_irqs[i], 0 );
        }
      else if( param->mcu->pinmap[i].typemask & PIN_IO )
        {
          state = GET_STATE( element->pin_vector[i] );
          if( state != param->state[i] )
            {
              param->state[i] = state;
              switch( state )
              {
                case SIG_HIGH: avr_raise_irq( param->wr_port_irqs[i], 1 ); break;
                case SIG_LOW: avr_raise_irq( param->wr_port_irqs[i], 0 ); break;
                default: break;
              }
            }
        }
    }
  return 0;
}
