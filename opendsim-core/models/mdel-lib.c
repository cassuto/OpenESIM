/** @file
 * model-lib
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

#define TRACE_UNIT "model-lib"

#include <dsim/logtrace.h>
#include <dsim/node-template.h>

#include "mdel-lib-internal.h"

extern circ_element_descriptor_t mdel_register;
extern circ_element_descriptor_t mdel_capacitor;
extern circ_element_descriptor_t mdel_diode;
extern circ_element_descriptor_t mdel_bjt;
extern circ_element_descriptor_t mdel_mosfet;
extern circ_element_descriptor_t mdel_opamp;
extern circ_element_descriptor_t mdel_inductor;
extern circ_element_descriptor_t mdel_vsource;
extern circ_element_descriptor_t mdel_dac;
extern circ_element_descriptor_t mdel_adc;
extern circ_element_descriptor_t mdel_or;
extern circ_element_descriptor_t mdel_xor;
extern circ_element_descriptor_t mdel_and;
extern circ_element_descriptor_t mdel_ddev;
extern circ_element_descriptor_t mdel_adev;

static const circ_element_descriptor_t *element_descriptors[] =
  {
    &mdel_register,
    &mdel_capacitor,
    &mdel_diode,
    &mdel_bjt,
    &mdel_mosfet,
    &mdel_opamp,
    &mdel_inductor,
    &mdel_vsource,
    &mdel_dac,
    &mdel_adc,
    &mdel_or,
    &mdel_xor,
    &mdel_and,
    &mdel_adev,
    &mdel_ddev
  };

static hashmap_t element_hashmap; /* voidptr_node */
static int ref_count = 0;

static int
model_lib_init( void )
{
  if( ref_count == 0 )
    {
      int rc;

      if( (rc = hashmap_init( &element_hashmap, HASHMAP_KEY_CHR, 0 )) )
        return rc;
      for( unsigned i=0; i<GET_ELEMENTS(element_descriptors); i++)
        {
          const char *symbol = element_descriptors[i]->symbol_name;
          if( !hashmap_contain_key( &element_hashmap, (hashmap_key_t)(symbol) ) )
              hashmap_new_at( &element_hashmap, (hashmap_key_t)(symbol), hashmap_const_voidptr_node_t )->val = element_descriptors[i];
          else
            {
              trace_error(("duplicated model symbol '%s'\n", symbol));
              return -DS_DUP_MODEL_SYMBOL;
            }
        }
    }
  ref_count ++;
  return 0;
}

static void
model_lib_release( void )
{
  if( --ref_count == 0 )
    {
      hashmap_release( &element_hashmap, ds_heap_free );
    }
  trace_assert( ref_count >= 0 );
}

int
model_create_instance( circ_element_t *element, const char *symbol, const circ_element_descriptor_t DS_OUT **ppdesc, const void DS_OUT **ppmdel )
{
  int rc = 0, param_count;

  if( (rc = model_lib_init()) ) /* lazy-init with reference counter */
    return rc;

  const circ_element_descriptor_t *desc;
  desc = hashmap_cast_at( &element_hashmap, (hashmap_key_t)symbol, hashmap_const_voidptr_node_t )->val;

  if( desc )
    {
      /* allocate parameter memory for the model */
      if( desc->param_size )
        {
          void *params = ds_heap_alloc( desc->param_size );
          if ( params )
            {
              memset( params, 0, desc->param_size );
              circ_element_set_param( element, params );
            }
          else
            {
              rc = -DS_NO_MEMORY;
              goto err_out;
            }
        }

      /* create pins required */
      if( desc->pin_count )
        {
          if( (rc = circ_element_set_pins( element, desc->pin_count )) )
            goto err_out;
        }

      switch( desc->mdel_type )
      {
        case MDEL_ANALOG:
          {
            asim_descriptor_t *asim = (asim_descriptor_t *)desc->mdel;

            if( (rc = asim->pfn_create( element )) )
              goto err_out;
            if( (rc = asim->pfn_config( element, ELM_CONFIG_LIST_COUNT, &param_count )) )
              goto err_out;

            *ppdesc = desc;
            *ppmdel = asim;
            rc = 0;
            break;
          }

        case MDEL_DIGITAL:
          {
            asim_descriptor_t *dsim = (asim_descriptor_t *)desc->mdel;

            if( (rc = dsim->pfn_create( element )) )
              goto err_out;

            /* query the number of parameters */
            if( (rc = dsim->pfn_config( element, ELM_CONFIG_LIST_COUNT, &param_count )) )
              goto err_out;

            *ppdesc = desc;
            *ppmdel = dsim;
            rc = 0;
            break;
          }
      }
    }
  else
    return -DS_FAULT;

  /* not found built-in models, see extended module */

  return rc;
err_out:
  return rc; /* no matched */

  (void)param_count;
}

void
model_destroy_instance( const circ_element_descriptor_t *desc, const void *mdel, circ_element_t *element )
{
  trace_assert ( desc && mdel && element );

  switch( desc->mdel_type )
  {
    case MDEL_ANALOG:
      asim_element_uninit( element );
      break;

    case MDEL_DIGITAL:
      dsim_element_uninit( element );
      break;
  }

  ds_heap_free( element->param );

  model_lib_release(); /* decrease the reference count */
}
