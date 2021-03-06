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

#include "dmcuavr.h"

int
LIB_FUNC(mcu_avr_config)( circ_element_t *element, int op, ... )
{
  DEFINE_PARAM(param, element, mcu_avr_param_t);
  int rc = 0;
  va_list vlist;
  va_start( vlist, op );

  UNUSED(param);

  switch( op )
  {
    case ELM_CONFIG_LIST_COUNT: /* Query the number of parameters */
      {
        int *count = va_arg( vlist, int* );
        *count = 6;
      }
      break;

    case ELM_CONFIG_LIST_PROP: /* Query the parameter property list */
      {
        const model_variable_prop_t **prop = va_arg( vlist, const model_variable_prop_t ** );
        static model_variable_prop_t prop_list[] =
            {
              { "mmcu", "Model of target MCU", MDEL_VAR_STRPTR },
              { "imgfn", "ROM image filename", MDEL_VAR_STRPTR },
              { "clk_freq", "(MHz) Frequency of system clock", MDEL_VAR_INTEGER },
              { "adc_imp", "Input impedance of ADC port", MDEL_VAR_DOUBLE },
              { "dac_imp", "Ouput impedance of DAC port", MDEL_VAR_DOUBLE },
              { "aref_imp", "Input impedance of AREF port", MDEL_VAR_DOUBLE }
            };
        *prop = prop_list;
      }
      break;

    case ELM_CONFIG_GET: /* Query the value of parameter */
      {
        int param_id = va_arg( vlist, int );
        switch ( param_id )
        {
          case 0: *(va_arg( vlist, const char** )) = param->mmcu; break;
          case 1: *(va_arg( vlist, const char** )) = param->romimg_fn; break;
          case 2: *(va_arg( vlist, int* )) = param->steps_pclk; break;
          case 3: *(va_arg( vlist, double* )) = param->adc_in_imp; break;
          case 4: *(va_arg( vlist, double* )) = param->dac_out_imp; break;
          case 5: *(va_arg( vlist, double* )) = param->aref_in_imp; break;
          default: rc = -DS_FAULT;
        }
      }
      break;

    case ELM_CONFIG_SET: /* Set the value of parameter and make it effective */
      {
        int param_id = va_arg( vlist, int );
        switch ( param_id )
        {
          case 0: /* Model of target MCU */
            {
              if( param->mmcu )
                {
                  rc = -DS_READONLY; break;
                }
              param->mmcu = ds_strdup( va_arg( vlist, const char* ) );
              if( param->mmcu )
                {
                  const mmcu_t *mcu = get_mmcu( param->mmcu );
                  if( mcu )
                    {
                      param->mcu = mcu;
                      param->analog_count = 0;
                      param->adc_channels = 0;
                      /*
                       * RESET type of pins
                       */
                      rc = circ_element_set_pins( element, mcu->pincount ); UPDATE_RC(rc);
                      for( int i=0; i<mcu->pincount; i++ )
                        {
                          if( param->mcu->pinmap[i].typemask & PIN_IO )
                            circ_pin_set_type( element->pin_vector[i], PIN_TYPE_DIGITAL );
                          if( param->mcu->pinmap[i].typemask & PIN_ADC )
                            {
                              circ_pin_set_type( element->pin_vector[i], PIN_TYPE_ANALOG );
                              param->analog_count++;
                              param->adc_channels++;
                            }
                          else if( (param->mcu->pinmap[i].typemask & PIN_VCC) ||
                              (param->mcu->pinmap[i].typemask & PIN_GND) ||
                              (param->mcu->pinmap[i].typemask & PIN_AREF) ||
                              (param->mcu->pinmap[i].typemask & PIN_AVCC) ||
                              (param->mcu->pinmap[i].typemask & PIN_DAC) )
                            {
                              param->analog_count++;
                            }
                        }
                    }
                  else
                    {
                      mdel_logtrace( MDEL_ERR, ("Register and pin mappings for processor \"%s\" is not found!", param->mmcu) );
                      rc = -DS_FAULT;
                    }
                }
              else
                rc = -DS_NO_MEMORY;
              break;
            }
          case 1: /* ROM image filename */
            {
              ds_heap_free( param->romimg_fn );
              param->romimg_fn = ds_strdup( va_arg( vlist, const char* ) );
              rc = param->romimg_fn ? 0 : -DS_NO_MEMORY;
              break;
            }
          case 2: /* (MHz) Frequency of system clock */
            {
              param->steps_pclk = va_arg( vlist, int );
              break;
            }
          case 3: /* Input impedance of ADC port */
            {
              param->adc_in_imp = va_arg( vlist, double );
              break;
            }
          case 4: /* Output impedance of DAC port */
            {
              param->dac_out_imp = va_arg( vlist, double );
              break;
            }
          case 5: /* Input impedance of AREF port */
            {
              param->aref_in_imp = va_arg( vlist, double );
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
