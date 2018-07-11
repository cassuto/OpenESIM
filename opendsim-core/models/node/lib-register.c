/** @file
 * model-lib Register Model
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

#include "mdel-lib-internal.h"

static int LIB_FUNC(register_create)( circ_element_t * );
static int LIB_FUNC(register_init)( circ_element_t * );
static int LIB_FUNC(register_stamp)( circ_element_t * );
static int LIB_FUNC(register_config)( circ_element_t *, int, ... );
static void LIB_FUNC(register_uninit)( circ_element_t * );

typedef struct
{
  double resist;
  double current;
} register_param_t;

asim_descriptor_t asim_register =
  {
    .pfn_create = register_create,      /* callback function to create parameters */

    .pfn_init = register_init,          /* callback function to initialize the simulator model */

    .pfn_stamp = register_stamp,        /* callback function to stamp node matrix */

    .pfn_vchanged = NULL,               /* callback function to inform volt changed */

    .pfn_config = register_config,      /* callback function to configure the element */

    .pfn_reset = NULL,                  /* callback function to reset the status */

    .pfn_uninit = register_uninit       /* callback function to terminate the parameters */
  };

circ_element_descriptor_t mdel_register =
  {
    .param_size = sizeof(register_param_t),

    .symbol_name = "R",                 /* symbol name used to identify this element */

    .pin_count = 2,                     /* the number of pins of device */

    .mdel_type = MDEL_ANALOG,          /* the electrical type of this model */

    .mdel = &asim_register              /* pointer to the model descriptor structure */
  };


static int
LIB_FUNC(register_create)( circ_element_t *element )
{
  DEFINE_PARAM(param, element, register_param_t);

  param->resist = 100.0f;
  param->current = 0.0f;

  return 0;
}

static int
LIB_FUNC(register_init)( circ_element_t *element )
{
  int rc = 0;

  if ( (rc = circ_pin_set_nodecomp( element->pin_vector[0], PINNODE(element, 1))) )
    return rc;
  return circ_pin_set_nodecomp( element->pin_vector[1], PINNODE(element, 0) );
}

static int
LIB_FUNC(register_stamp)( circ_element_t *element )
{
  DEFINE_PARAM(param, element, register_param_t);
  double admit = 1/param->resist;
  int rc;

  if( (rc = circ_node_stamp_admit( PINNODE(element, 0), element->pin_vector[0], admit )) )
    return rc;
  return circ_node_stamp_admit( PINNODE(element, 1), element->pin_vector[1], admit );
}

/* configuration procedure */
static int
LIB_FUNC(register_config)( circ_element_t *element, int op, ... )
{
  DEFINE_PARAM(param, element, register_param_t);
  int rc = 0;
  va_list vlist;
  va_start( vlist, op );

  switch( op )
  {
    case ELM_CONFIG_LIST_COUNT:
      {
        int *count = va_arg( vlist, int* );
        *count = 1;
      }
      break;

    case ELM_CONFIG_LIST_PROP:
      {
        const model_variable_prop_t **prop = va_arg( vlist, const model_variable_prop_t ** );
        static model_variable_prop_t prop_list[] =
            {
              { "r", "Resistance value", MDEL_VAR_DOUBLE }
            };
        *prop = prop_list;
      }
      break;

    case ELM_CONFIG_GET:
      {
        int param_id = va_arg( vlist, int );
        switch ( param_id )
        {
          case 0: *(va_arg( vlist, double* )) = param->resist; break;
          default: rc = -DS_FAULT;
        }
      }
      break;

    case ELM_CONFIG_SET:
      {
        int param_id = va_arg( vlist, int );
        switch ( param_id )
        {
          case 0: /* Resistance value */
            {
              double value = va_arg( vlist, double );

              if( value<1e-9 ) value = 1e-9;
              param->resist = value;
              register_stamp( element );
              break;
            }
          default:
            rc = -DS_FAULT;
        }
      }
      break;

    default:
      rc = -DS_UNIMPLEMENTED;
  }

  va_end( vlist );
  return rc;
}

static void
LIB_FUNC(register_uninit)( circ_element_t *element )
{
  DEFINE_PARAM(param, element, register_param_t);
  (void)param;
}
