/** @file
 * device-lib Register
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

#include "bjt_pnp.h"

DeviceLibraryEntry *dev_bjt_pnp::libraryEntry()
{
  return new DeviceLibraryEntry(
      /* symbol_name */     "BJT_PNP",
      /* reference_name */  "B?",
      /* category */        "Primitives",
      /* sub_category */    "BJTs",
      /* description */     "Generic BJT Device",
      /* manufacturer */    "BJT",
      /* symbolfile */      "bjt_pnp.ssym",
      dev_bjt_pnp::construct,
      0l);
}

IDevice *dev_bjt_pnp::construct( const char *reference, int id, void *reserved )
{
  return new dev_bjt_pnp( reference, id, reserved );
}

dev_bjt_pnp::dev_bjt_pnp( const char *reference, int id, void *reserved )
  : IDevice( reference, id, reserved )
{}

int dev_bjt_pnp::create( ISchematic *schematic, IPropertyContainer *properties )
{
  int rc = IDevice::baseinit( "BJT", m_circuit );               UPDATE_RC(rc);
  rc = properties->readDevice( this, 0 );                       UPDATE_RC(rc);

  UNUSED(schematic);
  return rc;
}

int dev_bjt_pnp::init( ISchematic *, IPropertyContainer *properties )
{
  int rc = circ_element_config(( m_mdel, ELM_CONFIG_SET, 1/*pnp*/, (int)true )); UPDATE_RC(rc);
      rc = properties->configDevice( this );
  return rc;
}

int dev_bjt_pnp::config( int op, ... )
{
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
              { "gain", "Gain value.", MDEL_VAR_INTEGER }
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
            rc = circ_element_config(( m_mdel, ELM_CONFIG_GET, 0/*gain*/, va_arg( vlist, int* ) ));
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
          case 0: /* Gain value */
            {
              rc = circ_element_config(( m_mdel, ELM_CONFIG_SET, 0/*gain*/, va_arg( vlist, int ) ));
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
