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

#include "opamp.h"

int
LIB_FUNC(opamp_init)( circ_element_t *element )
{
  int rc = 0;
  DEFINE_PARAM(param, element, opamp_param_t);

  param->accuracy = circuit_non_linear_accuracy(element->circuit);

  param->last_output = .0f;
  param->last_input  = .0f;
  param->k = 1e-6/param->gain;
  param->converged = true;

  if( element->pin_vector[0]->connected )
    if( (rc = circ_node_add_non_linear( PINNODE(element, 0), element )) )
      return rc;

  if( element->pin_vector[1]->connected )
    if( (rc = circ_node_add_non_linear( PINNODE(element, 1), element )) )
      return rc;

  if( element->pin_vector[2]->connected )
    if( (rc = circ_node_add_non_linear( PINNODE(element, 2), element )) )
      return rc;

  return 0;
}
