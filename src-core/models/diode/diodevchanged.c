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
LIB_FUNC(diode_vchanged)( circ_element_t *element )
{
  int rc;
  DEFINE_PARAM(param, element, diode_param_t);

  param->voltPN = GET_VOLT(element->pin_vector[0]) - GET_VOLT(element->pin_vector[1]);

  param->converged = true;

  double deltaR = param->imped;
  double deltaV = param->threshold;

  if( param->voltPN <= param->threshold )   /* Not conducing */
    {
      if( (param->zenerV > 0)&(param->voltPN <-param->zenerV) )
          deltaV =-param->zenerV;
      else
        {
          deltaV = param->voltPN;
          deltaR = model_high_imp;
        }
    }

  if( deltaR != param->resist )
    {
      param->resist = deltaR;
      if( (rc = diode_stamp( element )) )
        return rc;
      param->converged = false;
    }
  if( deltaV != param->deltaV )
    {
      param->deltaV = deltaV;

      double current = deltaV/param->resist;

      if( (rc = circ_node_stamp_current( PINNODE(element, 0), element->pin_vector[0], current )) )
        return rc;
      if( (rc = circ_node_stamp_current( PINNODE(element, 1), element->pin_vector[1], -current )) )
        return rc;
    }
  return 0;
}
