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

void
LIB_FUNC(mcu_avr_uninit)( circ_element_t *element )
{
  DEFINE_PARAM(param, element, mcu_avr_param_t);

  if( param->avr_processor )
    {
      avr_terminate( param->avr_processor );
    }
  ds_heap_free( param->mmcu );
  ds_heap_free( param->romimg_fn );
  ds_heap_free( param->state );
  ds_heap_free( param->port_irqs );
  ds_heap_free( param->ddr_irqs );
  ds_heap_free( param->wr_adc_irqs );
  ds_heap_free( param->adc_channel_pinid );

  if( param->innode )
    {
      for( int i=0; i < param->analog_count; i++ )
        {
          circ_node_free( param->innode[i] );
        }
    }
  ds_heap_free( param->innode );

  if( param->ports )
    {
      for( int i=0; i < circ_element_get_pin_count(element); i++ )
        {
          ds_heap_free( param->ports[i] );
        }
    }
  ds_heap_free( param->ports );
}
