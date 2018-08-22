/** @file
 * device-lib DC Vsource
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

#include "atmega164.h"

DeviceLibraryEntry *dev_atmega164::libraryEntry()
{
  return new DeviceLibraryEntry(
      /* symbol_name */     "Atmega164",
      /* reference_name */  "U?",
      /* category */        "MCUs",
      /* sub_category */    "AVR",
      /* description */     "ATMEL 8bit Microprocessor",
      /* manufacturer */    "ATMEL",
      /* symbolfile */      "atmega164.ssym",
      dev_atmega164::construct,
      0l);
}

IDevice *dev_atmega164::construct( const char *reference, int id, void *reserved )
{
  return new dev_atmega164( reference, id, reserved );
}

dev_atmega164::dev_atmega164( const char *reference, int id, void *reserved )
  : IDevice( reference, id, reserved )
{}

int dev_atmega164::create( ISchematic *schematic, IPropertyContainer *properties )
{
  int rc = IDevice::baseinit( "MCU_AVR", m_circuit );               UPDATE_RC(rc);

  rc = circ_element_config(( m_mdel, ELM_CONFIG_SET, 0/*mmcu*/, "atmega164" )); UPDATE_RC(rc);

  rc = properties->readDevice( this );                              UPDATE_RC(rc);

  UNUSED(schematic);
  return rc;
}

int dev_atmega164::init( ISchematic *, IPropertyContainer *properties )
{
  return properties->configDevice( this );
}

int dev_atmega164::config( int op, ... )
{
  int rc = 0;
  va_list vlist;
  va_start( vlist, op );

  switch( op )
  {
    case ELM_CONFIG_LIST_COUNT: /* Query the number of parameters */
      {
        int *count = va_arg( vlist, int* );
        *count = 2;
      }
      break;

    case ELM_CONFIG_LIST_PROP: /* Query the parameter property list */
      {
        const model_variable_prop_t **prop = va_arg( vlist, const model_variable_prop_t ** );
        static model_variable_prop_t prop_list[] =
            {
              { "imgfn", "ROM image filename", MDEL_VAR_STRPTR },
              { "clk_freq", "(MHz) Frequency of system clock", MDEL_VAR_INTEGER }
            };
        *prop = prop_list;
      }
      break;

    case ELM_CONFIG_GET: /* Query the value of parameter */
      {
        int param_id = va_arg( vlist, int );
        switch ( param_id )
        {
          case 0:
            rc = circ_element_config(( m_mdel, ELM_CONFIG_GET, 1/*imgfn*/, va_arg( vlist, const char** ) ));
            break;
          case 1:
            rc = circ_element_config(( m_mdel, ELM_CONFIG_GET, 2/*clk_freq*/, va_arg( vlist, int* ) ));
            break;
          default: rc = -DS_FAULT;
        }
      }
      break;

    case ELM_CONFIG_SET: /* Set the value of parameter and make it effective */
      {
        int param_id = va_arg( vlist, int );
        switch ( param_id )
        {
          case 1: /* ROM image filename */
            {
              rc = circ_element_config(( m_mdel, ELM_CONFIG_SET, 1/*imgfn*/, va_arg( vlist, const char* ) ));
              break;
            }
          case 2: /* (MHz) Frequency of system clock */
            {
              rc = circ_element_config(( m_mdel, ELM_CONFIG_SET, 2/*clk_freq*/, va_arg( vlist, int ) ));
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
