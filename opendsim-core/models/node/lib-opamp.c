/** @file
 * model-lib Operational Amplifier Model
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

static int LIB_FUNC(opamp_create)( circ_element_t * );
static int LIB_FUNC(opamp_init)( circ_element_t * );
static int LIB_FUNC(opamp_vchanged)( circ_element_t * );
static int LIB_FUNC(opamp_config)( circ_element_t *, int, ... );
static void LIB_FUNC(opamp_uninit)( circ_element_t * );

typedef struct
{
  bool converged;
  double accuracy;
  double gain;
  double k;
  double voltPos;
  double voltNeg;
  double last_output;
  double last_input;
} opamp_param_t;

asim_descriptor_t asim_opamp =
  {
    .pfn_create = opamp_create,         /* callback function to create parameters */

    .pfn_init = opamp_init,             /* callback function to initialize the simulator model */

    .pfn_stamp = NULL,                  /* callback function to stamp node matrix */

    .pfn_vchanged = opamp_vchanged,     /* callback function to inform volt changed */

    .pfn_config = opamp_config,         /* callback function to configure the element */

    .pfn_reset = NULL,                  /* callback function to reset the status */

    .pfn_uninit = opamp_uninit          /* callback function to terminate the parameters */
  };

circ_element_descriptor_t mdel_opamp =
  {
    .param_size = sizeof(opamp_param_t),

    .symbol_name = "OPAMP",             /* symbol name used to identify this element */

    .pin_count = 3,                     /* the number of pins of device */

    .mdel_type = MDEL_ANALOG,          /* the electrical type of this model */

    .mdel = &asim_opamp                 /* pointer to the model descriptor structure */
  };


static int
LIB_FUNC(opamp_create)( circ_element_t *element )
{
  DEFINE_PARAM(param, element, opamp_param_t);

  param->gain = 1000.0f;

  return 0;
}

static int
LIB_FUNC(opamp_init)( circ_element_t *element )
{
  int rc = 0;
  DEFINE_PARAM(param, element, opamp_param_t);

  param->accuracy = circuit_non_linear_accuracy(element->circuit);

  param->last_output = .0f;
  param->last_input  = .0f;
  param->k = 1e-6/param->gain;
  param->converged = true;

  if( element->pin_vector[0]->connected )
    if( (rc = circ_node_add_non_linear( PINNODE(element, 0), element )) )
      return rc;

  if( element->pin_vector[1]->connected )
    if( (rc = circ_node_add_non_linear( PINNODE(element, 1), element )) )
      return rc;

  if( element->pin_vector[2]->connected )
    if( (rc = circ_node_add_non_linear( PINNODE(element, 2), element )) )
      return rc;

  return 0;
}

static int
LIB_FUNC(opamp_vchanged)( circ_element_t *element )
{
  DEFINE_PARAM(param, element, opamp_param_t);

  int rc;
  double vd = GET_VOLT( element->pin_vector[0] ) - GET_VOLT( element->pin_vector[1] );

  double out = vd * param->gain;
  if     ( out > param->voltPos ) out = param->voltPos;
  else if( out < param->voltNeg ) out = param->voltNeg;

  if( fabs(out-param->last_output) < param->accuracy )
    {
      param->converged = true;
      return 0;
    }

  if( param->converged )                  /* First step after a convergence */
    {
      double dOut = -1e-6;
      if( vd>0 ) dOut = 1e-6;

      out = param->last_output + dOut;
      param->converged = false;
    }
  else
    {
      if( param->last_input != vd ) /* We problably are in a close loop configuration */
        {
          double dIn  = fabs(param->last_input-vd); /* Input diff with last step */

          /* Guess next converging output: */
          out = (param->last_output*dIn + vd*1e-6)/(dIn + param->k);
        }
      param->converged = true;
    }

  if     ( out >= param->voltPos ) out = param->voltPos;
  else if( out <= param->voltNeg ) out = param->voltNeg;

  param->last_input = vd;
  param->last_output = out;

  return circ_node_stamp_current( PINNODE(element, 2), element->pin_vector[2], out/model_cero_doub );
}

/* configuration procedure */
static int
LIB_FUNC(opamp_config)( circ_element_t *element, int op, ... )
{
  DEFINE_PARAM(param, element, opamp_param_t);
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
              { "g", "Gain value", MDEL_VAR_DOUBLE }
            };
        *prop = prop_list;
      }
      break;

    case ELM_CONFIG_GET:
      {
        int param_id = va_arg( vlist, int );
        switch ( param_id )
        {
          case 0: *(va_arg( vlist, double* )) = param->gain; break;
          default: rc = -DS_FAULT;
        }
      }
      break;

    case ELM_CONFIG_SET:
      {
        int param_id = va_arg( vlist, int );
        switch ( param_id )
        {
          case 0: /* Set Gain value */
            {
              param->gain = va_arg( vlist, double );
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
LIB_FUNC(opamp_uninit)( circ_element_t *element )
{
  DEFINE_PARAM(param, element, opamp_param_t);
  (void)param;
}
