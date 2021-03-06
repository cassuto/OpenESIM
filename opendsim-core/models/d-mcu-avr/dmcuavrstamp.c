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

#include "dmcuavr.h"

int
LIB_FUNC(mcu_avr_stamp)( circ_element_t *element )
{
  int rc;
  int ports = circ_element_get_pin_count(element);
  DEFINE_PARAM(param, element, mcu_avr_param_t);

  for( int i=0; i < ports; i++ )
    {
      if( param->mcu->pinmap[i].typemask & PIN_ADC )
        {
          if( element->pin_vector[i]->connected )
            if( (rc = circ_node_stamp_admit( PINNODE(element, i), element->pin_vector[i], 1/param->adc_in_imp )) )
              return rc;
        }
      else if( param->mcu->pinmap[i].typemask & PIN_DAC )
        {
          if( element->pin_vector[i]->connected )
            if( (rc = circ_node_stamp_admit( PINNODE(element, i), element->pin_vector[i], 1/param->dac_out_imp )) )
              return rc;
        }
      else if( param->mcu->pinmap[i].typemask & PIN_AREF )
        {
          if( element->pin_vector[i]->connected )
            if( (rc = circ_node_stamp_admit( PINNODE(element, i), element->pin_vector[i], 1/param->aref_in_imp )) )
              return rc;
        }
      else if( param->mcu->pinmap[i].typemask & PIN_VCC )
        {
          if( element->pin_vector[i]->connected )
            if( (rc = circ_node_stamp_admit( PINNODE(element, i), element->pin_vector[i], 1/param->vcc_in_imp )) )
              return rc;
        }
    }
  return 0;
}
