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
LIB_FUNC(opamp_vchanged)( circ_element_t *element )
{
  DEFINE_PARAM(param, element, opamp_param_t);

  int rc;
  double vd = GET_VOLT( element->pin_vector[0] ) - GET_VOLT( element->pin_vector[1] );

  double out = vd * param->gain;
  if     ( out > param->voltPos ) out = param->voltPos;
  else if( out < param->voltNeg ) out = param->voltNeg;

  if( fabs(out-param->last_output) < param->accuracy )
    {
      param->converged = true;
      return 0;
    }

  if( param->converged )                  /* First step after a convergence */
    {
      double dOut = -1e-6;
      if( vd>0 ) dOut = 1e-6;

      out = param->last_output + dOut;
      param->converged = false;
    }
  else
    {
      if( param->last_input != vd ) /* We problably are in a close loop configuration */
        {
          double dIn  = fabs(param->last_input-vd); /* Input diff with last step */

          /* Guess next converging output: */
          out = (param->last_output*dIn + vd*1e-6)/(dIn + param->k);
        }
      param->converged = true;
    }

  if     ( out >= param->voltPos ) out = param->voltPos;
  else if( out <= param->voltNeg ) out = param->voltNeg;

  param->last_input = vd;
  param->last_output = out;

  return circ_node_stamp_current( PINNODE(element, 2), element->pin_vector[2], out/model_cero_doub );
}
