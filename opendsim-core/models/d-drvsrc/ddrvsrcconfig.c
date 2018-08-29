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

#include "d-drvsrc.h"

int
LIB_FUNC(drvsrc_config)( circ_element_t *element, int op, ... )
{
  DEFINE_PARAM(param, element, drvsrc_param_t);
  int rc = 0;
  va_list vlist;
  va_start( vlist, op );

  UNUSED(param);

  switch( op )
  {
    case ELM_CONFIG_LIST_COUNT: /* Query the number of parameters */
      {
        int *count = va_arg( vlist, int* );
        *count = 1;
      }
      break;

    case ELM_CONFIG_LIST_PROP: /* Query the parameter property list */
      {
        const model_variable_prop_t **prop = va_arg( vlist, const model_variable_prop_t ** );
        static model_variable_prop_t prop_list[] =
            {
              { "gate", "High output gate", MDEL_VAR_BOOL }
            };
        *prop = prop_list;
      }
      break;

    case ELM_CONFIG_GET: /* Query the value of parameter */
      {
        int param_id = va_arg( vlist, int );
        switch ( param_id )
        {
          case 0: *(va_arg( vlist, int* )) = param->state; break;
          default: rc = -DS_FAULT;
        }
      }
      break;

    case ELM_CONFIG_SET: /* Set the value of parameter and make it effective */
      {
        int param_id = va_arg( vlist, int );
        switch ( param_id )
        {
          case 0: /* High output gate */
            {
              param->state = (logic_state_t)va_arg( vlist, int );
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
