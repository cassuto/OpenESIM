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

#include "inductor.h"

int
LIB_FUNC(inductor_create)( circ_element_t *element )
{
  DEFINE_PARAM(param, element, inductor_param_t);

  param->ind = 1; /* H */
  param->dT = (double)circuit_react_clock(element->circuit)/1e6;
  param->resist = param->ind/param->dT;
  param->current_source = .0f;
  param->volt = .0f;

  return 0;
}
