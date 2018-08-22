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
LIB_FUNC(mcu_avr_clock)( circ_element_t *element )
{
  DEFINE_PARAM(param, element, mcu_avr_param_t);

  if( !param->avr_processor ) return 0;
  while( param->avr_processor->cycle < param->next_cycle )
    {
      param->avr_processor->run( param->avr_processor );
    }
  param->next_cycle += param->steps_pclk;

  return 0;
}
