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

#include "dac.h"

int
LIB_FUNC(dac_config)( circ_element_t *element, int op, ... )
{
  DEFINE_PARAM(param, element, dac_param_t);
  int rc = 0;
  va_list vlist;
  va_start( vlist, op );

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
              { "inc", "Number of Input port", MDEL_VAR_INTEGER },
              { "vh", "Logic Volt high",    MDEL_VAR_DOUBLE },
              { "vl",  "Logic Volt low",    MDEL_VAR_DOUBLE },
              { "imp", "Output Impedance", MDEL_VAR_DOUBLE },
              { "inv", "Input inverted", MDEL_VAR_BOOL }
            };
        *prop = prop_list;
      }
      break;

    case ELM_CONFIG_GET: /* Query the value of parameter */
      {
        int param_id = va_arg( vlist, int );
        switch ( param_id )
        {
          case 0: *(va_arg( vlist, int* )) = param->outputs_count; break;
          case 1: *(va_arg( vlist, double* )) = param->output_vh; break;
          case 2: *(va_arg( vlist, double* )) = param->output_vl; break;
          case 3: *(va_arg( vlist, double* )) = param->imp; break;
          case 4:
            {
              int pin_index = va_arg( vlist, int );
              *(va_arg( vlist, int* )) = (int)(param->inverted[pin_index]);
              break;
            }
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
              if( (rc = circ_element_set_pins( element, n * 2 )) )
                break;
              rc = dac_set_vsrcs( element, param, n );
              break;
            }
          case 1: /* Logic Volt high */
            {
              param->output_vh = va_arg( vlist, double );
              break;
            }
          case 2: /* Logic Volt low */
            {
              param->output_vl = va_arg( vlist, double );
              break;
            }
          case 3: /* Output Impedance */
            {
              double admit; int index;
              param->imp = va_arg( vlist, double );
              admit = 1/param->imp;
              for( int i=0; i < param->outputs_count; i++)
                {
                  index = param->outputs_count + i;
                  if( (rc = circ_node_stamp_admit( PINNODE(element, index), element->pin_vector[index], admit )) )
                    break;
                }
              break;
            }
          case 4: /* Output Inverted */
            {
              int pin_index = va_arg( vlist, int );
              param->inverted[pin_index] = (bool)(va_arg( vlist, int ));
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
