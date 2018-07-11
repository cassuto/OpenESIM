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
LIB_FUNC(inductor_init)( circ_element_t *element )
{
  int rc = 0;
  DEFINE_PARAM(param, element, inductor_param_t);

  if( element->pin_vector[0]->connected )
    if( (rc = circ_node_add_react( PINNODE(element, 0), element)) )
      return rc;

  if( element->pin_vector[1]->connected )
    if( (rc = circ_node_add_react( PINNODE(element, 1), element)) )
      return rc;

  param->dT = (double)circuit_react_clock(element->circuit)/1e6;

  if( (rc = set_resist( element, param, param->ind/param->dT )) )
    return rc;

  if( (rc = circ_pin_set_nodecomp( element->pin_vector[0], PINNODE(element, 1))) )
    return rc;
  if( (rc = circ_pin_set_nodecomp( element->pin_vector[1], PINNODE(element, 0) )) )
    return rc;

  param->current_source = 0;
  return 0;
}
