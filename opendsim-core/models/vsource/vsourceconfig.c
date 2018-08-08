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

#include "vsource.h"

int
LIB_FUNC(vsource_config)( circ_element_t *element, int op, ... )
{
  DEFINE_PARAM(param, element, vsource_param_t);
  int rc = 0;
  va_list vlist;
  va_start( vlist, op );

  switch( op )
  {
    case ELM_CONFIG_LIST_COUNT: /* Query the number of parameters */
      {
        int *count = va_arg( vlist, int* );
        *count = 4;
      }
      break;

    case ELM_CONFIG_LIST_PROP: /* Query the parameter property list */
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

    case ELM_CONFIG_GET: /* Query the value of parameter */
      {
        int param_id = va_arg( vlist, int );
        switch ( param_id )
        {
          case 0: *(va_arg( vlist, double* )) = param->vh; break;
          case 1: *(va_arg( vlist, double* )) = param->vl; break;
          case 2: *(va_arg( vlist, int* )) = (int)param->out; break;
          case 3: *(va_arg( vlist, double* )) = param->imp; break;
          default: rc = -DS_FAULT;
        }
      }
      break;

    case ELM_CONFIG_SET: /* Set the value of parameter and make it effective */
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
