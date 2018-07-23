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

#include "capacitor.h"

int
LIB_FUNC(capacitor_vchanged)( circ_element_t *element )
{
  DEFINE_PARAM(param, element, capacitor_param_t);

  int rc;
  double volt = GET_VOLT(element->pin_vector[0]) - GET_VOLT(element->pin_vector[1]);

  if( fabs(volt) < 1e-9 ) return 0;

  double current_source = volt/param->resist;

  if( (rc = circ_node_stamp_current( PINNODE(element, 0), element->pin_vector[0], current_source )) )
    return rc;
  return circ_node_stamp_current( PINNODE(element, 1), element->pin_vector[1], -current_source );
}
