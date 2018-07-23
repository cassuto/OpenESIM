/** @file
 * model-lib Voltage Source Model
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

static int LIB_FUNC(vsource_create)( circ_element_t * );
static int LIB_FUNC(vsource_init)( circ_element_t * );
static int LIB_FUNC(vsource_stamp)( circ_element_t * );
static int LIB_FUNC(vsource_config)( circ_element_t *, int, ... );
static void LIB_FUNC(vsource_uninit)( circ_element_t * );

typedef struct
{
  double vh;
  double vl;
  double volt_out;
  double imp;
  double admit;
  bool out;
  circ_node_t *src_node;
} vsource_param_t;

asim_descriptor_t asim_vsource =
  {
    .pfn_create = vsource_create,       /* callback function to create parameters */

    .pfn_init = vsource_init,           /* callback function to initialize the simulator model */

    .pfn_stamp = vsource_stamp,         /* callback function to stamp node matrix */

    .pfn_vchanged = NULL,               /* callback function to inform volt changed */

    .pfn_config = vsource_config,       /* callback function to configure the element */

    .pfn_reset = NULL,                  /* callback function to reset the status */

    .pfn_uninit = vsource_uninit        /* callback function to terminate the parameters */
  };

circ_element_descriptor_t mdel_vsource =
  {
    .param_size = sizeof(vsource_param_t),

    .symbol_name = "VS",                /* symbol name used to identify this element */

    .pin_count = 1,                     /* the number of pins of device */

    .mdel_type = MDEL_ANALOG,          /* the electrical type of this model */

    .mdel = &asim_vsource               /* pointer to the model descriptor structure */
  };


static int
LIB_FUNC(vsource_create)( circ_element_t *element )
{
  DEFINE_PARAM(param, element, vsource_param_t);

  param->vh = model_cero_doub;
  param->vl  = model_cero_doub;
  param->volt_out  = model_cero_doub;
  param->imp      = model_cero_doub;
  param->admit    = 1/param->imp;

  param->src_node = circ_node_create( element->circuit, true );
  if( param->src_node )
    {
      circ_node_set_index( param->src_node, 0 );
      return 0;
    }
  return -DS_NO_MEMORY;
}

static int
LIB_FUNC(vsource_init)( circ_element_t *element )
{
  DEFINE_PARAM(param, element, vsource_param_t);

  int rc = circ_pin_set_nodecomp( element->pin_vector[0], param->src_node );
  if( rc ) return rc;

  return vsource_stamp( element );
}

static int
vsource_stamp_output( circ_element_t *element )
{
  int rc;
  DEFINE_PARAM(param, element, vsource_param_t);

  if( (rc = circ_node_set_volt( param->src_node, param->volt_out )) )
    return rc;

  if( (rc = circ_node_stamp_current( PINNODE(element, 0), element->pin_vector[0], param->volt_out/param->imp )) )
    return rc;

  return 0;
}

static int
LIB_FUNC(vsource_stamp)( circ_element_t *element )
{
  DEFINE_PARAM(param, element, vsource_param_t);
  int rc = circ_node_stamp_admit( PINNODE(element, 0), element->pin_vector[0], param->admit );
  if( rc ) return rc;

  return vsource_stamp_output( element );
}

/* configuration procedure */
static int
LIB_FUNC(vsource_config)( circ_element_t *element, int op, ... )
{
  DEFINE_PARAM(param, element, vsource_param_t);
  int rc = 0;
  va_list vlist;
  va_start( vlist, op );

  switch( op )
  {
    case ELM_CONFIG_LIST_COUNT:
      {
        int *count = va_arg( vlist, int* );
        *count = 4;
      }
      break;

    case ELM_CONFIG_LIST_PROP:
      {
        const model_variable_prop_t **prop = va_arg( vlist, const model_variable_prop_t ** );
        static model_variable_prop_t prop_list[] =
            {
              { "vh", "Volt high",        MDEL_VAR_DOUBLE },
              { "vl",  "Volt low",         MDEL_VAR_DOUBLE },
              { "g", "Volt output gate", MDEL_VAR_BOOL },
              { "imp", "Internal Impedance", MDEL_VAR_DOUBLE }
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
          case 0: *value = param->vh; break;
          case 1: *value = param->vl; break;
          case 2: *value = param->volt_out; break;
          case 3: *value = param->imp; break;
          default: rc = -DS_FAULT;
        }
      }
      break;

    case ELM_CONFIG_SET:
      {
        int param_id = va_arg( vlist, int );
        switch ( param_id )
        {
          case 0: /* Vh = Volt high */
            {
              double value = va_arg( vlist, double );
              param->vh = value;
              if( param->out ) param->volt_out = value;
              rc = vsource_stamp_output( element );
              break;
            }
          case 1: /* Vl = Volt low */
            {
              double value = va_arg( vlist, double );
              param->vl = value;
              if( !param->out ) param->volt_out = value;
              rc = vsource_stamp_output( element );
              break;
            }
          case 2: /* g = Volt output gate */
            {
              bool gate = (bool)va_arg( vlist, int );
              if( gate ) param->volt_out = param->vh;
              else       param->volt_out = param->vl;
              rc = vsource_stamp_output( element );
              break;
            }
          case 3: /* imp = Internal Impedance */
            {
              double value = va_arg( vlist, double );
              param->imp = value;
              param->admit = 1/param->imp;
              rc = vsource_stamp( element );
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
LIB_FUNC(vsource_uninit)( circ_element_t *element )
{
  DEFINE_PARAM(param, element, vsource_param_t);

  circ_node_free( param->src_node );
}
