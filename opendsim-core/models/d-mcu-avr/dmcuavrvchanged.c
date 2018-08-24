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
LIB_FUNC(mcu_avr_vchanged)( circ_element_t *element )
{
  int ports = circ_element_get_pin_count(element);
  DEFINE_PARAM(param, element, mcu_avr_param_t);

  for( int i=0; i < ports; i++ )
    {
      double volt = GET_VOLT( element->pin_vector[i] );

      if( param->mcu->pinmap[i].typemask & PIN_ADC)
        return mcu_avr_event( element );
      else if( param->mcu->pinmap[i].typemask & PIN_AREF )
        param->avr_processor->aref = volt * 1000;
      else if( param->mcu->pinmap[i].typemask & PIN_AVCC )
        param->avr_processor->avcc = volt * 1000;
      else if( param->mcu->pinmap[i].typemask & PIN_VCC )
        param->avr_processor->vcc = volt * 1000;
    }

  return 0;
}
