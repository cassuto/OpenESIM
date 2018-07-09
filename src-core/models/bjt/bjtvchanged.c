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
LIB_FUNC(bjt_vchanged)( circ_element_t *element )
{
  DEFINE_PARAM(param, element, bjt_param_t);

  int rc;
  double voltCE;
  double voltBE;
  double voltC = GET_VOLT( element->pin_vector[0] );
  double voltE = GET_VOLT( element->pin_vector[1] );
  double voltB = GET_VOLT( element->pin_vector[2] );

  if( param->is_PNP )
    {
      voltCE = voltE-voltC;
      voltBE = voltE-voltB;
    }
  else
    {
      voltCE = voltC-voltE;
      voltBE = voltB-voltE;
    }
  if( voltCE < model_cero_doub ) voltCE = model_cero_doub;

  double maxCurrCE = voltCE/param->resist;
  double current = maxCurrCE;

  //if( voltBE > 0.6 )
  {
      if(( param->E_follow == false)&( fabs(voltE) > 1e-3 ))
        {
          if(( fabs(param->voltE) > 1e-3 )&( param->voltE != voltE )){param->E_follow = true;}
          param->voltE = voltE;
        }
      double satK = 0;

      if( voltCE < voltBE )
        {
          satK = voltCE/voltBE-1;
          satK = pow( satK, 2 );
        }
      param->base_curr = pn_current(param->BE_juct);

      double currentCE = param->base_curr*param->gain*(1+voltCE/75);
      currentCE -= currentCE*satK;
      if( param->E_follow ) currentCE /= 2;

      if( currentCE > maxCurrCE )
        {
          param->resist = voltCE/currentCE;
          bjt_stamp(element);
        }
      current = maxCurrCE-currentCE;
  }
  if( param->is_PNP ) current = -current;

  if( fabs(current-param->last_out)<param->accuracy ) return 0;

  param->last_out = current;
  if( (rc = circ_node_stamp_current( PINNODE(element, 0), element->pin_vector[0], current )) )
    return rc;
  if( (rc = circ_node_stamp_current( PINNODE(element, 1), element->pin_vector[1], -current )) )
    return rc;

  return 0;
}
