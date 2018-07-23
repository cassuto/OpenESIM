/** @file
 * device-lib Device base class
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

#define TRACE_UNIT "dev-lib"

#include <dsim/logtrace.h>
#include <device/device.h>

#include <dsim/error.h>
#include <dsim/circuit.h>
#include <model/circ-element.h>

IDevice::IDevice( const char *reference_, int id_, circuit_t *circuit_, void *reserved_ )
  : reserved( 0 ),
    m_reference( reference_ ),
    m_id( id_ ),
    m_mdel( 0 ),
    m_circuit( 0 )
{
  UNUSED(reserved_);
}

IDevice::~IDevice()
{}

/* set up main mdel */
int IDevice::baseinit( const char *mdel_symbol )
{
  trace_assert( NULL==m_mdel );

  if( mdel_symbol )
    {
      m_mdel = circ_element_create( m_circuit, mdel_symbol, m_id );
      if( m_mdel )
        {
          circuit_attach_element( m_circuit, m_mdel );
          return 0;
        }
      return -DS_NO_MEMORY;
    }
  return 0;
}

/* set up inline mdel */
int IDevice::baseinit( device_type type )
{
  int rc;

  trace_assert( NULL==m_mdel );

  switch( type )
  {
    case DEV_ANALOG:
      m_mdel = circ_element_create( m_circuit, "_ADEV_", 1 );
      break;

    case DEV_DIGITAL:
      m_mdel = circ_element_create( m_circuit, "_DDEV_", 1 );
      break;
  }

  if( m_mdel )
    {
      if( (rc = circ_element_config(( m_mdel, /*pIDevice*/0, (void*)this ))) )
        return rc;

      circuit_attach_element( m_circuit, m_mdel );
      return 0;
    }

  return -DS_NO_MEMORY;
}

int IDevice::pin_count()
{
  return m_mdel->pin_count;
}

circ_pin_t * IDevice::pin( int index )
{
  return m_mdel->pin_vector[index];
}

struct IRECT IDevice::get_bound()
{
  return IRECT( 0, 0, 0, 0 );
}

int IDevice::mdel_init()
  { return 0; }
int IDevice::mdel_stamp()
  { return 0; }
int IDevice::mdel_vchanged()
  { return 0; }
int IDevice::mdel_event()
  { return 0; }
void IDevice::mdel_reset()
  {}
void IDevice::mdel_uninit()
  {}

