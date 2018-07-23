/** @file
 * model-lib PN junction Model
 */

/*
 *
 * Equivalent Broken line model:
 *  Id
 *   ^
 *   |          /
 *   |         /
 *   |        /
 *   |_ _ _ _/
 *   0-------------> Vd
 *         Vth
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

static int LIB_FUNC(pn_create)( circ_element_t * );
static int LIB_FUNC(pn_init)( circ_element_t * );
static int LIB_FUNC(pn_stamp)( circ_element_t * );
static int LIB_FUNC(pn_vchanged)( circ_element_t * );
static int LIB_FUNC(pn_config)( circ_element_t *, int, ... );
static void LIB_FUNC(pn_uninit)( circ_element_t * );

typedef struct
{
  double resist;
  double accuracy;
  double voltPN;
  double deltaV;
  double threshold;
} pn_param_t;

asim_descriptor_t asim_pn =
  {
    .pfn_create = pn_create,            /* callback function to create parameters */

    .pfn_init = pn_init,                /* callback function to initialize the simulator model */

    .pfn_stamp = pn_stamp,              /* callback function to stamp node matrix */

    .pfn_vchanged = pn_vchanged,        /* callback function to inform volt changed */

    .pfn_config = pn_config,            /* callback function to configure the element */

    .pfn_reset = NULL,                  /* callback function to reset the status */

    .pfn_uninit = pn_uninit             /* callback function to terminate the parameters */
  };

circ_element_descriptor_t mdel_pn =
  {
    .param_size = sizeof(pn_param_t),

    .symbol_name = "_PN_",              /* symbol name used to identify this element */

    .pin_count = 2,                     /* the number of pins of device */

    .mdel_type = MDEL_ANALOG,          /* the electrical type of this model */

    .mdel = &asim_pn                    /* pointer to the model descriptor structure */
  };


static int
LIB_FUNC(pn_create)( circ_element_t *element )
{
  DEFINE_PARAM(param, element, pn_param_t);

  param->threshold = 0.7f;
  param->resist = 0.6f;

  return 0;
}

static int
LIB_FUNC(pn_init)( circ_element_t *element )
{
  int rc = 0;
  DEFINE_PARAM(param, element, pn_param_t);

  param->accuracy = circuit_non_linear_accuracy(element->circuit);
  param->voltPN = .0f;
  param->deltaV  = .0f;

  if( (rc = circ_node_add_non_linear( PINNODE(element, 0), element )) )
    return rc;
  if( (rc = circ_node_add_non_linear( PINNODE(element, 1), element )) )
    return rc;

  if ( (rc = circ_pin_set_nodecomp( element->pin_vector[0], PINNODE(element, 1))) )
    return rc;
  return circ_pin_set_nodecomp( element->pin_vector[1], PINNODE(element, 0) );
}

static int
LIB_FUNC(pn_stamp)( circ_element_t *element )
{
  DEFINE_PARAM(param, element, pn_param_t);

  double admit = 1/param->resist;
  int rc;

  if( (rc = circ_node_stamp_admit( PINNODE(element, 0), element->pin_vector[0], admit )) )
    return rc;
  return circ_node_stamp_admit( PINNODE(element, 1), element->pin_vector[1], admit );
}

static int
LIB_FUNC(pn_vchanged)( circ_element_t *element )
{
  DEFINE_PARAM(param, element, pn_param_t);

  int rc;
  param->voltPN = GET_VOLT(element->pin_vector[0]) - GET_VOLT(element->pin_vector[1]);

  double deltaV = param->threshold;

  if( param->voltPN < param->threshold ) deltaV = param->voltPN;

  if( fabs(deltaV-param->deltaV) < param->accuracy ) return 0;

  param->deltaV = deltaV;

  double current = deltaV/param->resist;

  if( (rc = circ_node_stamp_current( PINNODE(element, 0), element->pin_vector[0], current )) )
    return rc;
  if( (rc = circ_node_stamp_current( PINNODE(element, 1), element->pin_vector[1], -current )) )
    return rc;

  return 0;
}

/* configuration procedure */
static int
LIB_FUNC(pn_config)( circ_element_t *element, int op, ... )
{
  DEFINE_PARAM(param, element, pn_param_t);
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
              { "vt", "Threshold voltage",        MDEL_VAR_DOUBLE },
            };
        *prop = prop_list;
      }
      break;

    case ELM_CONFIG_GET:
      {
        int param_id = va_arg( vlist, int );
        double *value = va_arg( vlist, double* );
        switch ( param_id )
        {
          case 0: *value = param->threshold; break;
          default: rc = -DS_FAULT;
        }
      }
      break;

    case ELM_CONFIG_SET:
      {
        int param_id = va_arg( vlist, int );
        switch ( param_id )
        {
          case 0: /* Threshold voltage */
            {
              param->threshold = va_arg( vlist, double );
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
LIB_FUNC(pn_uninit)( circ_element_t *element )
{
  DEFINE_PARAM(param, element, pn_param_t);
  (void)param;
}


/* Get the current of PN junction */
double
LIB_INT_FUNC(pn_current)( circ_element_t *element )
{
  DEFINE_PARAM(param, element, pn_param_t);

  double current = .0f;

  if( element->pin_vector[0]->connected && element->pin_vector[1]->connected )
    {
      double volt = param->voltPN - param->deltaV;
      if( volt>0 )
        {
          current = volt/param->resist;
        }
    }
  return current;
}
