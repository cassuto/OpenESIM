/** @file
 * model-lib MOSFET Model
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

static int LIB_FUNC(mosfet_create)( circ_element_t * );
static int LIB_FUNC(mosfet_init)( circ_element_t * );
static int LIB_FUNC(mosfet_stamp)( circ_element_t * );
static int LIB_FUNC(mosfet_vchanged)( circ_element_t * );
static int LIB_FUNC(mosfet_config)( circ_element_t *, int, ... );
static void LIB_FUNC(mosfet_uninit)( circ_element_t * );

typedef struct
{
  double resist;
  double accuracy;
  double last_current;
  double threshold;
  double kRDSon;
  double RDSon;
  double gateV;
  double Gth;
  double Vs;

  bool Pchannel;
  bool S_follow;
} mosfet_param_t;

asim_descriptor_t asim_mosfet =
  {
    .pfn_create = mosfet_create,        /* callback function to create parameters */

    .pfn_init = mosfet_init,            /* callback function to initialize the simulator model */

    .pfn_stamp = mosfet_stamp,          /* callback function to stamp node matrix */

    .pfn_vchanged = mosfet_vchanged,    /* callback function to inform volt changed */

    .pfn_config = mosfet_config,        /* callback function to configure the element */

    .pfn_reset = NULL,                  /* callback function to reset the status */

    .pfn_uninit = mosfet_uninit         /* callback function to terminate the parameters */
  };

circ_element_descriptor_t mdel_mosfet =
  {
    .param_size = sizeof(mosfet_param_t),

    .symbol_name = "MOS",               /* symbol name used to identify this element */

    .pin_count = 3,                     /* the number of pins of device */

    .mdel_type = MDEL_ANALOG,          /* the electrical type of this model */

    .mdel = &asim_mosfet                /* pointer to the model descriptor structure */
  };


static int
LIB_FUNC(mosfet_create)( circ_element_t *element )
{
  DEFINE_PARAM(param, element, mosfet_param_t);

  param->Pchannel = false;

  param->gateV     = .0f;
  param->resist    = 1.0f;
  param->RDSon     = 1.0f;
  param->threshold = 3.0f;

  return -DS_NO_MEMORY;
}

static inline int
set_resist( circ_element_t *element, mosfet_param_t *param, double resist )
{
  param->resist = resist;
  return mosfet_stamp( element );
}

static int
LIB_FUNC(mosfet_init)( circ_element_t *element )
{
  int rc = 0;
  DEFINE_PARAM(param, element, mosfet_param_t);

  if( (rc = set_resist( element, param, 1.0f )) )
    return rc;

  param->accuracy = circuit_non_linear_accuracy(element->circuit);

  param->last_current = .0f;
  param->Vs = .0f;
  param->S_follow = false;

  param->kRDSon = param->RDSon*(10-param->threshold);
  param->Gth = param->threshold-param->threshold/4;

  if( (element->pin_vector[0]->connected)
    & (element->pin_vector[1]->connected)
    & (element->pin_vector[2]->connected) )
    {
      if( (rc = circ_node_add_non_linear( PINNODE(element, 0), element )) )
        return rc;
      if( (rc = circ_node_add_non_linear( PINNODE(element, 1), element )) )
        return rc;
      if( (rc = circ_node_add_non_linear( PINNODE(element, 2), element )) )
        return rc;
    }

  if ( (rc = circ_pin_set_nodecomp( element->pin_vector[0], PINNODE(element, 1))) )
    return rc;
  return circ_pin_set_nodecomp( element->pin_vector[1], PINNODE(element, 0) );
}

static int
LIB_FUNC(mosfet_stamp)( circ_element_t *element )
{
  DEFINE_PARAM(param, element, mosfet_param_t);

  double admit = 1/param->resist;
  int rc;

  if( (rc = circ_node_stamp_admit( PINNODE(element, 0), element->pin_vector[0], admit )) )
    return rc;
  return circ_node_stamp_admit( PINNODE(element, 1), element->pin_vector[1], admit );
}

static int
LIB_FUNC(mosfet_vchanged)( circ_element_t *element )
{
  DEFINE_PARAM(param, element, mosfet_param_t);

  int rc;
  double Vgs;
  double Vds;
  double Vd = GET_VOLT( element->pin_vector[0] );
  double Vs = GET_VOLT( element->pin_vector[1] );
  double Vg = GET_VOLT( element->pin_vector[2] );

  if(( param->S_follow == false)&( fabs(Vs) > 1e-3 ))
    {
      if(( fabs(param->Vs) > 1e-3 )&( param->Vs != Vs )) param->S_follow = true;
      param->Vs = Vs;
    }

  if( param->Pchannel )
    {
      Vgs = Vs-Vg;
      Vds = Vs-Vd;
    }
  else
    {
      Vgs = Vg-Vs;
      Vds = Vd-Vs;
    }
  param->gateV = Vgs - param->Gth;

  if( param->gateV < 0 ) param->gateV = 0;

  double satK = 1+Vds/100;
  double maxCurrDS = Vds/param->resist;

  if( Vds > param->gateV ) Vds = param->gateV;

  double DScurrent = (param->gateV*Vds-pow(Vds,2)/2)*satK/param->kRDSon;
  //if( param->Sfollow ) DScurrent /= 2;
  if( DScurrent > maxCurrDS ) DScurrent = maxCurrDS;

  double current = maxCurrDS-DScurrent;
  if( param->Pchannel ) current = -current;

  if( fabs(current-param->last_current)<param->accuracy ) return 0;

  param->last_current = current;
  if( (rc = circ_node_stamp_current( PINNODE(element, 0), element->pin_vector[0], current )) )
    return rc;
  if( (rc = circ_node_stamp_current( PINNODE(element, 1), element->pin_vector[1], -current )) )
    return rc;

  return 0;
}

/* configuration procedure */
static int
LIB_FUNC(mosfet_config)( circ_element_t *element, int op, ... )
{
  DEFINE_PARAM(param, element, mosfet_param_t);
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
              { "rdson", "Resistance of RD when ON.", MDEL_VAR_DOUBLE },
              { "Vth", "Threshold voltage", MDEL_VAR_DOUBLE },
              { "pch", "P Channel ?", MDEL_VAR_BOOL }
            };
        *prop = prop_list;
      }
      break;

    case ELM_CONFIG_GET:
      {
        int param_id = va_arg( vlist, int );
        switch ( param_id )
        {
          case 0: *(va_arg( vlist, double* )) = param->RDSon; break;
          case 1: *(va_arg( vlist, double* )) = param->threshold; break;
          case 2: *(va_arg( vlist, int* )) = (int)(param->Pchannel); break;
          default: rc = -DS_FAULT;
        }
      }
      break;

    case ELM_CONFIG_SET:
      {
        int param_id = va_arg( vlist, int );
        switch ( param_id )
        {
          case 0: /* Set RDS(ON) value */
            {
              double val = va_arg( vlist, double );
              if( val < model_cero_doub ) val = model_cero_doub;
              if( val > 10.0f ) val = 10.0f;
              param->RDSon = val;
              break;
            }
          case 1: /* Set Threshold voltage */
            {
              param->threshold = va_arg( vlist, double );
              break;
            }
          case 2: /* Set P Channel boolean */
            {
              param->Pchannel = (bool)va_arg( vlist, int );
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
LIB_FUNC(mosfet_uninit)( circ_element_t *element )
{
  DEFINE_PARAM(param, element, mosfet_param_t);
  (void)param;
}
