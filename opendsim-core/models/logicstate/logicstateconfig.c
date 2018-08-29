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

#include "logicstate.h"

int
LIB_FUNC(logicstate_config)( circ_element_t *element, int op, ... )
{
  DEFINE_PARAM(param, element, logicstate_param_t);
  int rc = 0;
  va_list vlist;
  va_start( vlist, op );

  UNUSED(param);

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
              { "inc", "Number of Input port", MDEL_VAR_INTEGER },
              { "curpin", "Current pin index", MDEL_VAR_INTEGER},
              { "state", "State of current pin", MDEL_VAR_INTEGER}
            };
        *prop = prop_list;
      }
      break;

    case ELM_CONFIG_GET: /* Query the value of parameter */
      {
        int param_id = va_arg( vlist, int );
        switch ( param_id )
        {
          case 0: *(va_arg( vlist, int* )) = circ_element_get_pin_count(element) / 2; break;
          case 1: *(va_arg( vlist, int*)) = param->curpin; break;
          case 2: *(va_arg( vlist, int*)) = (int)param->state[param->curpin]; break;
          default: rc = -DS_FAULT;
        }
      }
      break;

    case ELM_CONFIG_SET: /* Set the value of parameter and make it effective */
      {
        int param_id = va_arg( vlist, int );
        switch ( param_id )
        {
          case 0: /* Number of Input ports */
            {
              int n = va_arg( vlist, int );
              if( (rc = circ_element_set_pins( element, n )) )
                break;
              circ_element_set_digital_pin( element, 0, n-1 );
              break;
            }
          case 1: /* Current pin index */
            {
              param->curpin = va_arg( vlist, int );
              break;
            }
          case 2: /* State of current pin */
            rc = -DS_READONLY;
            break;
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
