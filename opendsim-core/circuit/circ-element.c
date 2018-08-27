/** @file
 * Circuit element
 */

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

#define TRACE_UNIT "circ-elm"

#include <dsim/logtrace.h>
#include <dsim/memory.h>
#include <dsim/error.h>
#include <dsim/string.h>
#include <dsim/circuit.h>
#include <model/model.h>
#include <model/circ-pin.h>

#include <model/circ-element.h>


circ_element_t *
circ_element_create( circuit_t *circuit, const char *symbol, int id )
{
  circ_element_t *element = (circ_element_t *)ds_heap_alloc( sizeof(*element) );
  if ( element )
    {
      memset( element, 0, sizeof(*element) );
      element->id = id;
      element->pin_vector = NULL;
      element->pin_count = 0;
      element->param = NULL;
      element->circuit = circuit;

      if( model_create_instance( element, symbol, &element->desc, &(element->u.ptr) ) )
        {
          ds_heap_free( element );
          return NULL;
        }
    }
  return element;
}

int
circ_element_set_pins( circ_element_t *element, int n )
{
  circ_pin_t *pin;

  if ( element->pin_count )
    {
      for( int i=0; i<element->pin_count; i++ )
        {
          if ( element->pin_vector[i] )
            circ_pin_free( element->pin_vector[i] );
        }
      ds_heap_free( element->pin_vector );
    }

  element->pin_vector = (circ_pin_t**)ds_heap_alloc( sizeof(circ_pin_t*) * n );

  if ( element->pin_vector )
    {
      for( int i=0; i<n; i++ )
        {
          pin = circ_pin_create();
          if( pin )
            {
              circ_pin_set_index( pin, i );
              element->pin_vector[i] = pin;
            }
          else return -DS_NO_MEMORY;
        }
      element->pin_count = n;
      return 0;
    }
  element->pin_count = 0;
  return -DS_NO_MEMORY;
}

circ_pin_t *
circ_element_get_pin( circ_element_t *element, int index )
{
  return element->pin_vector[index];
}

void
circ_element_set_digital_pin( circ_element_t *element, int start, int end )
{
  trace_assert( start < element->pin_count && end < element->pin_count );
  trace_assert( start <= end );
  for( int i=start; i <= end; i++ )
    {
      circ_pin_set_type( element->pin_vector[i], PIN_TYPE_DIGITAL );
    }
}

void
circ_element_free( void *ptr )
{
  circ_element_t *element = (circ_element_t *)ptr;
  if ( element )
    {
      model_destroy_instance( element->desc, element->u.ptr, element );

      for( int i=0; i<element->pin_count; i++ )
        {
          if ( element->pin_vector[i] )
            circ_pin_free( element->pin_vector[i] );
        }
      ds_heap_free( element->pin_vector );
    }
  ds_heap_free( element );
}
