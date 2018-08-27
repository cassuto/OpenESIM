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

#include "bjt.h"

int
LIB_FUNC(bjt_init)( circ_element_t *element )
{
  int rc = 0;
  DEFINE_PARAM(param, element, bjt_param_t);

  if( param->gain <= 0 ) param->gain = 100;
  if( (rc = set_resist( element, param, 400/param->gain )) )
    return rc;

  param->accuracy = circuit_non_linear_accuracy(element->circuit);
  param->last_out = .0f;
  param->base_curr = .0f;
  param->voltE = .0f;
  param->E_follow = false;

  param->voltPN  = .0f;

  if( element->pin_vector[0]->connected ) /* C */
    {
      if( (rc = circ_node_add_non_linear( PINNODE(element, 0), element )) )
        return rc;
    }
  if( element->pin_vector[1]->connected ) /* E */
    {
      circ_node_t* enod1 = PINNODE(element, 1);
      if( (rc = circ_node_add_non_linear( enod1, element )) )
        return rc;

      if( param->is_PNP )
        {
          rc = circ_pin_set_node( param->BE_juct->pin_vector[0], enod1 );
        }
      else
        {
          rc = circ_pin_set_node( param->BE_juct->pin_vector[1], enod1 );
        }
      if( rc ) return rc;
    }
  if( element->pin_vector[2]->connected ) /* B */
   {
      circ_node_t* enod2 = PINNODE(element, 2);
      if( (rc = circ_node_add_non_linear( enod2, element )) )
        return rc;

      if( param->is_PNP )
        {
          rc = circ_pin_set_node( param->BE_juct->pin_vector[1], enod2 );
        }
      else
        {
          rc = circ_pin_set_node( param->BE_juct->pin_vector[0], enod2 );
        }
      if( rc ) return rc;
   }

  if ( (rc = circ_pin_set_nodecomp( element->pin_vector[0], PINNODE(element, 1))) )
    return rc;
  return circ_pin_set_nodecomp( element->pin_vector[1], PINNODE(element, 0) );
}
