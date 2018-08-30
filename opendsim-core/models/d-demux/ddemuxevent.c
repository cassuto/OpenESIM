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

#include "d-demux.h"

static int bindec_table[3] = { 1, 2, 4 };

int
LIB_FUNC(demux_event)( circ_element_t *element )
{
  int port = 0;
  for( int i=8; i<11; i++ )
    {
      if( SIG_HIGH == GET_STATE( element->pin_vector[i] ) ) port += bindec_table[i-8];
    }

  logic_state_t out = GET_STATE( element->pin_vector[12] );
  if( SIG_LOW == GET_STATE( element->pin_vector[11] ) )
    {
      circ_pin_set_state( element->pin_vector[port], out );
      for( int i=0; i<8; i++ )
        {
          if( i != port ) circ_pin_set_state( element->pin_vector[i], SIG_LOW );
        }
    }
  else
    {
      for( int i=0; i<8; i++ )
        {
          circ_pin_set_state( element->pin_vector[i], SIG_FLOAT );
        }
    }

  return 0;
}
