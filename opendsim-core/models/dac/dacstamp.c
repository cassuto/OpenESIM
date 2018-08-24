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
LIB_FUNC(dac_stamp)( circ_element_t *element )
{
  int rc, index;
  DEFINE_PARAM(param, element, dac_param_t);
  double admit = 1/param->imp; double volt;

  for( int i=0; i < param->outputs_count; i++)
    {
      index = param->outputs_count + i;
      volt = param->volt[i];

      if( element->pin_vector[index]->connected )
        if( (rc = circ_node_stamp_admit( PINNODE(element, index), element->pin_vector[index], admit )) )
          return rc;

      if( (rc = circ_node_set_volt( param->vsrc_node[i], volt )) )
        return rc;

      if( element->pin_vector[index]->connected )
        if( (rc = circ_node_stamp_current( PINNODE(element, index), element->pin_vector[index], volt/param->imp )) )
          return rc;
    }

  return 0;
}
