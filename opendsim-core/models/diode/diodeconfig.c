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

#include "diode.h"

int
LIB_FUNC(diode_config)( circ_element_t *element, int op, ... )
{
  DEFINE_PARAM(param, element, diode_param_t);
  int rc = 0;
  va_list vlist;
  va_start( vlist, op );

  switch( op )
  {
    case ELM_CONFIG_LIST_COUNT: /* Query the number of parameters */
      {
        int *count = va_arg( vlist, int* );
        *count = 3;
      }
      break;

    case ELM_CONFIG_LIST_PROP: /* Query the parameter property list */
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

    case ELM_CONFIG_GET: /* Query the value of parameter */
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

    case ELM_CONFIG_SET: /* Set the value of parameter and make it effective */
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
