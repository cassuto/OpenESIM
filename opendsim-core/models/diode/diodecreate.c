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

#include "diode.h"

int
LIB_FUNC(diode_create)( circ_element_t *element )
{
  DEFINE_PARAM(param, element, diode_param_t);

  param->resist = model_high_imp;
  param->imped = 0.6;
  param->threshold = 0.7;
  param->zenerV = .0f;
  param->resist = model_high_imp;
  return 0;
}
