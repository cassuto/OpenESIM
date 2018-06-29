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
LIB_FUNC(dac_create)( circ_element_t *element )
{
  DEFINE_PARAM(param, element, dac_param_t);

  param->output_vh  = model_logic_vh;
  param->output_vl  = model_logic_vl;
  param->imp       = model_cero_doub;
  param->outputs_count = 1;

  return dac_set_vsrcs( element, param, param->outputs_count );
}
