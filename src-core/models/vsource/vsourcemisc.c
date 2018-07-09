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
LIB_INT_FUNC(vsource_stamp_output)( circ_element_t *element )
{
  int rc;
  DEFINE_PARAM(param, element, vsource_param_t);

  if( (rc = circ_node_set_volt( param->src_node, param->volt_out )) )
    return rc;

  if( (rc = circ_node_stamp_current( PINNODE(element, 0), element->pin_vector[0], param->volt_out/param->imp )) )
    return rc;

  return 0;
}
