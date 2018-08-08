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

#include "vsource.h"

int
LIB_FUNC(vsource_create)( circ_element_t *element )
{
  DEFINE_PARAM(param, element, vsource_param_t);

  param->vh = model_cero_doub;
  param->vl  = model_cero_doub;
  param->volt_out  = model_cero_doub;
  param->imp      = model_cero_doub;
  param->admit    = 1/param->imp;
  param->out      = true;

  param->src_node = circ_node_create( element->circuit, true );
  if( param->src_node )
    {
      circ_node_set_index( param->src_node, 0 );
      return 0;
    }
  return -DS_NO_MEMORY;
}
