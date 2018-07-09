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
LIB_FUNC(mosfet_vchanged)( circ_element_t *element )
{
  DEFINE_PARAM(param, element, mosfet_param_t);

  int rc;
  double Vgs;
  double Vds;
  double Vd = GET_VOLT( element->pin_vector[0] );
  double Vs = GET_VOLT( element->pin_vector[1] );
  double Vg = GET_VOLT( element->pin_vector[2] );

  if(( param->S_follow == false)&( fabs(Vs) > 1e-3 ))
    {
      if(( fabs(param->Vs) > 1e-3 )&( param->Vs != Vs )) param->S_follow = true;
      param->Vs = Vs;
    }

  if( param->Pchannel )
    {
      Vgs = Vs-Vg;
      Vds = Vs-Vd;
    }
  else
    {
      Vgs = Vg-Vs;
      Vds = Vd-Vs;
    }
  param->gateV = Vgs - param->Gth;

  if( param->gateV < 0 ) param->gateV = 0;

  double satK = 1+Vds/100;
  double maxCurrDS = Vds/param->resist;

  if( Vds > param->gateV ) Vds = param->gateV;

  double DScurrent = (param->gateV*Vds-pow(Vds,2)/2)*satK/param->kRDSon;
  //if( param->Sfollow ) DScurrent /= 2;
  if( DScurrent > maxCurrDS ) DScurrent = maxCurrDS;

  double current = maxCurrDS-DScurrent;
  if( param->Pchannel ) current = -current;

  if( fabs(current-param->last_current)<param->accuracy ) return 0;

  param->last_current = current;
  if( (rc = circ_node_stamp_current( PINNODE(element, 0), element->pin_vector[0], current )) )
    return rc;
  if( (rc = circ_node_stamp_current( PINNODE(element, 1), element->pin_vector[1], -current )) )
    return rc;

  return 0;
}
