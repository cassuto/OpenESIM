/** @file
 * model-lib Diode Model
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
 *
 *
 * (unimplemented) Equivalent small-signal model: (1+, 2-)
 *      ---1---
 *     |       |        Q( Id = ID, Vd = VD ):
 *     --      v        Rd = dVd / dId = VT/Id
 *  Rd ||     (x) Ieq
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

static int LIB_FUNC(diode_create)( circ_element_t * );
static int LIB_FUNC(diode_init)( circ_element_t * );
static int LIB_FUNC(diode_vchanged)( circ_element_t * );
static int LIB_FUNC(diode_stamp)( circ_element_t * );
static int LIB_FUNC(diode_config)( circ_element_t *, int, ... );
static void LIB_FUNC(diode_uninit)( circ_element_t * );

typedef struct
{
  double resist;
  bool converged;
  double voltPN;
  double deltaV;
  double current;
  double threshold;
  double imped;
  double zenerV;
} diode_param_t;

asim_descriptor_t asim_diode =
  {
    .pfn_create = diode_create,         /* callback function to create parameters */

    .pfn_init = diode_init,             /* callback function to initialize the simulator model */

    .pfn_stamp = diode_stamp,           /* callback function to stamp node matrix */

    .pfn_vchanged = diode_vchanged,     /* callback function to inform volt changed */

    .pfn_config = diode_config,         /* callback function to configure the element */

    .pfn_reset = NULL,                  /* callback function to reset the status */

    .pfn_uninit = diode_uninit          /* callback function to terminate the parameters */
  };

circ_element_descriptor_t mdel_diode =
  {
    .param_size = sizeof(diode_param_t),

    .symbol_name = "D",                /* symbol name used to identify this element */

    .pin_count = 2,                     /* the number of pins of device */

    .mdel_type = MDEL_ANALOG,         /* the electrical type of this model */

    .mdel = &asim_diode                /* pointer to the model descriptor structure */
  };


static int
LIB_FUNC(diode_create)( circ_element_t *element )
{
  DEFINE_PARAM(param, element, diode_param_t);

  param->resist = model_high_imp;
  param->imped = 0.6;
  param->threshold = 0.7;
  param->zenerV = .0f;
  param->resist = model_high_imp;
  return 0;
}

static int
LIB_FUNC(diode_init)( circ_element_t *element )
{
  int rc = 0;
  DEFINE_PARAM(param, element, diode_param_t);

  param->resist = model_high_imp;
  param->converged = true;
  param->voltPN  = .0f;
  param->deltaV  = .0f;
  param->current = .0f;

  if( element->pin_vector[0]->connected )
    if( (rc = circ_node_add_non_linear( PINNODE(element, 0), element )) )
      return rc;

  if( element->pin_vector[1]->connected )
    if( (rc = circ_node_add_non_linear( PINNODE(element, 1), element )) )
      return rc;

  if ( (rc = circ_pin_set_nodecomp( element->pin_vector[0], PINNODE(element, 1))) )
    return rc;
  return circ_pin_set_nodecomp( element->pin_vector[1], PINNODE(element, 0) );
}

static int
LIB_FUNC(diode_stamp)( circ_element_t *element )
{
  DEFINE_PARAM(param, element, diode_param_t);
  double admit = 1/param->resist;
  int rc;

  if( (rc = circ_node_stamp_admit( PINNODE(element, 0), element->pin_vector[0], admit )) )
    return rc;
  return circ_node_stamp_admit( PINNODE(element, 1), element->pin_vector[1], admit );
}

static int
LIB_FUNC(diode_vchanged)( circ_element_t *element )
{
  int rc;
  DEFINE_PARAM(param, element, diode_param_t);

  param->voltPN = GET_VOLT(element->pin_vector[0]) - GET_VOLT(element->pin_vector[1]);

  param->converged = true;

  double deltaR = param->imped;
  double deltaV = param->threshold;

  if( param->voltPN <= param->threshold )   /* Not conducing */
    {
      if( (param->zenerV > 0)&(param->voltPN <-param->zenerV) )
          deltaV =-param->zenerV;
      else
        {
          deltaV = param->voltPN;
          deltaR = model_high_imp;
        }
    }

  if( deltaR != param->resist )
    {
      param->resist = deltaR;
      if( (rc = diode_stamp( element )) )
        return rc;
      param->converged = false;
    }
  if( deltaV != param->deltaV )
    {
      param->deltaV = deltaV;

      double current = deltaV/param->resist;

      if( (rc = circ_node_stamp_current( PINNODE(element, 0), element->pin_vector[0], current )) )
        return rc;
      if( (rc = circ_node_stamp_current( PINNODE(element, 1), element->pin_vector[1], -current )) )
        return rc;
    }
  return 0;
}

static int
set_resist( diode_param_t *param, circ_element_t *element, double resist )
{
  if( resist == 0 ) resist = 0.1;
  param->imped = resist;
  return diode_vchanged( element );
}

/* configuration procedure */
static int
LIB_FUNC(diode_config)( circ_element_t *element, int op, ... )
{
  DEFINE_PARAM(param, element, diode_param_t);
  int rc = 0;
  va_list vlist;
  va_start( vlist, op );

  switch( op )
  {
    case ELM_CONFIG_LIST_COUNT:
      {
        int *count = va_arg( vlist, int* );
        *count = 3;
      }
      break;

    case ELM_CONFIG_LIST_PROP:
      {
        const model_variable_prop_t **prop = va_arg( vlist, const model_variable_prop_t ** );
        static model_variable_prop_t prop_list[] =
            {
              { "vth", "Threshold voltage", MDEL_VAR_DOUBLE },
              { "imp", "Impedance value", MDEL_VAR_DOUBLE },
              { "vz", "Zener voltage", MDEL_VAR_DOUBLE }
            };
        *prop = prop_list;
      }
      break;

    case ELM_CONFIG_GET:
      {
        switch ( va_arg( vlist, int ) )
        {
          case 0: *(va_arg( vlist, double* )) = param->threshold; break;
          case 1: *(va_arg( vlist, double* )) = param->imped; break;
          case 2: *(va_arg( vlist, double* )) = param->zenerV; break;
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
          case 1: /* Resistance value */
            {
              rc = set_resist( param, element, va_arg( vlist, double ) );
              break;
            }
          case 2: /* Zener voltage */
            {
              double zenerV = va_arg( vlist, double );
              if( zenerV > 0 ) param->zenerV = zenerV;
              else             param->zenerV = 0;
              rc = set_resist( param, element, param->imped );
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
LIB_FUNC(diode_uninit)( circ_element_t *element )
{
  DEFINE_PARAM(param, element, diode_param_t);
  (void)param;
}
