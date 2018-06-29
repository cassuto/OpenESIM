/** @file
 * model-lib Inductor Model
 */

/*
 * Equivalent model:
 *      ---1---
 *     |       |
 *     --      v            Req = L/dT
 * Req ||     (x) Ieq       Ieq(n) = Ieq(n-1) + V(n-1)/Req
 *     --      |
 *     |       |
 *      ---2---
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

static int LIB_FUNC(inductor_create)( circ_element_t * );
static int LIB_FUNC(inductor_init)( circ_element_t * );
static int LIB_FUNC(inductor_stamp)( circ_element_t * );
static int LIB_FUNC(inductor_vchanged)( circ_element_t *element );
static int LIB_FUNC(inductor_config)( circ_element_t *, int, ... );
static void LIB_FUNC(inductor_uninit)( circ_element_t * );

typedef struct
{
  double resist;
  double ind;
  double current_source;
  double dT;
  double volt;
} inductor_param_t;

asim_descriptor_t asim_inductor =
  {
    .pfn_create = inductor_create,      /* callback function to create parameters */

    .pfn_init = inductor_init,          /* callback function to initialize the simulator model */

    .pfn_stamp = inductor_stamp,        /* callback function to stamp node matrix */

    .pfn_vchanged = inductor_vchanged,  /* callback function to inform volt changed */

    .pfn_config = inductor_config,      /* callback function to configure the element */

    .pfn_reset = NULL,                  /* callback function to reset the status */

    .pfn_uninit = inductor_uninit       /* callback function to terminate the parameters */
  };

circ_element_descriptor_t mdel_inductor =
  {
    .param_size = sizeof(inductor_param_t),

    .symbol_name = "L",                 /* symbol name used to identify this element */

    .pin_count = 2,                     /* the number of pins of device */

    .mdel_type = MDEL_ANALOG,          /* the electrical type of this model */

    .mdel = &asim_inductor              /* pointer to the model descriptor structure */
  };


static int
LIB_FUNC(inductor_create)( circ_element_t *element )
{
  DEFINE_PARAM(param, element, inductor_param_t);

  param->ind = 1; /* H */
  param->dT = (double)circuit_react_clock(element->circuit)/1e6;
  param->resist = param->ind/param->dT;
  param->current_source = .0f;
  param->volt = .0f;

  return 0;
}

static int
set_resist( circ_element_t *element, inductor_param_t *param, double value )
{
  if( value<1e-9 ) value = 1e-9;
  param->resist = value;
  return inductor_stamp( element );
}

static int
LIB_FUNC(inductor_init)( circ_element_t *element )
{
  int rc = 0;
  DEFINE_PARAM(param, element, inductor_param_t);

  if( element->pin_vector[0]->connected )
    if( (rc = circ_node_add_react( PINNODE(element, 0), element)) )
      return rc;

  if( element->pin_vector[1]->connected )
    if( (rc = circ_node_add_react( PINNODE(element, 1), element)) )
      return rc;

  param->dT = (double)circuit_react_clock(element->circuit)/1e6;

  if( (rc = set_resist( element, param, param->ind/param->dT )) )
    return rc;

  if( (rc = circ_pin_set_nodecomp( element->pin_vector[0], PINNODE(element, 1))) )
    return rc;
  if( (rc = circ_pin_set_nodecomp( element->pin_vector[1], PINNODE(element, 0) )) )
    return rc;

  param->current_source = 0;
  return 0;
}

static int
LIB_FUNC(inductor_vchanged)( circ_element_t *element )
{
  int rc = 0;
  DEFINE_PARAM(param, element, inductor_param_t);

  double volt = GET_VOLT(element->pin_vector[0]) - GET_VOLT(element->pin_vector[1]);

  if( fabs(volt) < 1e-9 ) return 0;

  param->current_source += volt/param->resist;
  if( (rc = circ_node_stamp_current( PINNODE(element, 0), element->pin_vector[0], param->current_source )) )
    return rc;
  return circ_node_stamp_current( PINNODE(element, 1), element->pin_vector[1], -param->current_source );
}

static int
LIB_FUNC(inductor_stamp)( circ_element_t *element )
{
  DEFINE_PARAM(param, element, inductor_param_t);
  double admit = 1/param->resist;
  int rc;

  if( (rc = circ_node_stamp_admit( PINNODE(element, 0), element->pin_vector[0], admit )) )
    return rc;
  return circ_node_stamp_admit( PINNODE(element, 1), element->pin_vector[1], admit );
}

/* configuration procedure */
static int
LIB_FUNC(inductor_config)( circ_element_t *element, int op, ... )
{
  DEFINE_PARAM(param, element, inductor_param_t);
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
              { "ind", "Inductance value", MDEL_VAR_DOUBLE }
            };
        *prop = prop_list;
      }
      break;

    case ELM_CONFIG_GET:
      {
        int param_id = va_arg( vlist, int );
        switch ( param_id )
        {
          case 0: *(va_arg( vlist, double* )) = param->ind; break;
          default: rc = -DS_FAULT;
        }
      }
      break;

    case ELM_CONFIG_SET:
      {
        int param_id = va_arg( vlist, int );
        switch ( param_id )
        {
          case 0: /* Inductance value */
            {
              param->ind = va_arg( vlist, double );
              rc = set_resist( element, param, param->ind/param->dT );
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
LIB_FUNC(inductor_uninit)( circ_element_t *element )
{
  DEFINE_PARAM(param, element, inductor_param_t);
  (void)param;
}
