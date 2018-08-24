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
#include <dsim/error.h>
#include <dsim/memory.h>
#include <dsim/circuit.h>
#include <device/device.h>
#include <model/circ-element.h>

#include <cstdarg>

IDevice::IDevice( const char *reference_, int id_, void *reserved_ )
  : reserved( 0 ),
    m_reference( reference_ ),
    m_id( id_ ),
    m_mdel( 0 ),
    m_circuit( 0l )
{
  UNUSED(reserved_);
}

IDevice::~IDevice() {}

circ_element_t *IDevice::createModel( const char *mdel_symbol, circuit_t *circuit )
{
  circ_element_t *m_mdel = circ_element_create( circuit, mdel_symbol, m_id );
  if( m_mdel )
    {
      circuit_attach_element( circuit, m_mdel );
      return m_mdel;
    }
  return 0l;
}

/* set up main mdel */
int IDevice::baseinit( const char *mdel_symbol, circuit_t *circuit )
{
  trace_assert( NULL==m_mdel );

  if( mdel_symbol )
    {
      m_mdel = circ_element_create( circuit, mdel_symbol, m_id );
      if( m_mdel )
        {
          circuit_attach_element( circuit, m_mdel );
          return 0;
        }
      return -DS_CREATE_MODEL;
    }
  return 0;
}

/* set up inline mdel */
int IDevice::baseinit( device_type type, circuit_t *circuit )
{
  int rc;

  trace_assert( NULL==m_mdel );

  switch( type )
  {
    case DEV_ANALOG:
      m_mdel = circ_element_create( circuit, "_ADEV_", 1 );
      break;

    case DEV_DIGITAL:
      m_mdel = circ_element_create( circuit, "_DDEV_", 1 );
      break;
  }

  if( m_mdel )
    {
      if( (rc = circ_element_config(( m_mdel, /*pIDevice*/0, (void*)this ))) )
        return rc;

      circuit_attach_element( circuit, m_mdel );
      return 0;
    }

  return -DS_NO_MEMORY;
}

void IDevice::baseuninit()
{ m_mdel = 0l; }

int IDevice::pin_count()
{
  return m_mdel->pin_count;
}

circ_pin_t * IDevice::pin( int index )
{
  return m_mdel->pin_vector[index];
}

int IDevice::config( int op, ... )
{
  int rc = 0;
  va_list vlist;
  va_start( vlist, op );

  switch( op )
  {
    case ELM_CONFIG_LIST_COUNT: /* Query the number of parameters */
      {
        *(va_arg( vlist, int* )) = 0;
      }
      break;

    default:
      rc = -DS_UNIMPLEMENTED;
  }

  va_end( vlist );
  return rc;
}

void IDevice::setCircuit( circuit_t *circuit ) { m_circuit = circuit; }


int IDevice::mdel_init()
  { return 0; }
int IDevice::mdel_stamp()
  { return 0; }
int IDevice::mdel_vchanged()
  { return 0; }
void IDevice::mdel_reset()
  {}
void IDevice::mdel_uninit()
  {}
int IDevice::mdel_clock()
  { return 0; }
int IDevice::mdel_event()
  { return 0; }
