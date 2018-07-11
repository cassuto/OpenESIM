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

#include "mosfet.h"

int
LIB_FUNC(mosfet_init)( circ_element_t *element )
{
  int rc = 0;
  DEFINE_PARAM(param, element, mosfet_param_t);

  if( (rc = set_resist( element, param, 1.0f )) )
    return rc;

  param->accuracy = circuit_non_linear_accuracy(element->circuit);

  param->last_current = .0f;
  param->Vs = .0f;
  param->S_follow = false;

  param->kRDSon = param->RDSon*(10-param->threshold);
  param->Gth = param->threshold-param->threshold/4;

  if( (element->pin_vector[0]->connected)
    & (element->pin_vector[1]->connected)
    & (element->pin_vector[2]->connected) )
    {
      if( (rc = circ_node_add_non_linear( PINNODE(element, 0), element )) )
        return rc;
      if( (rc = circ_node_add_non_linear( PINNODE(element, 1), element )) )
        return rc;
      if( (rc = circ_node_add_non_linear( PINNODE(element, 2), element )) )
        return rc;
    }

  if ( (rc = circ_pin_set_nodecomp( element->pin_vector[0], PINNODE(element, 1))) )
    return rc;
  return circ_pin_set_nodecomp( element->pin_vector[1], PINNODE(element, 0) );
}
