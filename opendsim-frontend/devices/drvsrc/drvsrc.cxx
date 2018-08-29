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

#include "drvsrc.h"

DeviceLibraryEntry *dev_drvsrc::libraryEntry()
{
  return new DeviceLibraryEntry(
      /* symbol_name */     "drvsrc",
      /* reference_name */  "dsrc?",
      /* category */        "Primitives",
      /* sub_category */    "Sources",
      /* description */     "Generic Logical Driver Source",
      /* manufacturer */    "Sources",
      /* symbolfile */      "drvsrc.ssym",
      dev_drvsrc::construct,
      0l);
}

IDevice *dev_drvsrc::construct( const char *reference, int id, void *reserved )
{
  return new dev_drvsrc( reference, id, reserved );
}

dev_drvsrc::dev_drvsrc( const char *reference, int id, void *reserved )
  : IDevice( reference, id, reserved )
  , m_state( (logic_state_t)-1 )
{}

int dev_drvsrc::create( ISchematic *schematic, IPropertyContainer *properties )
{
  int rc = IDevice::baseinit( "DRVSRC", m_circuit );            UPDATE_RC(rc);
  rc = properties->readDevice( this );                          UPDATE_RC(rc);

  UNUSED(schematic);
  return rc;
}

int dev_drvsrc::init( ISchematic *schematic, IPropertyContainer *properties )
{
  schematic->registerRender( this );
  schematic->acceptRenderEvent( true );
  return properties->configDevice( this );
}

#define READOUT_LABEL_X     (3)
#define READOUT_LABEL_Y     (13)
#define READOUT_LABEL_SIZE  (16)

int dev_drvsrc::render_frame( ISchematic *, IDeviceGraph *deviceGraph )
{
  logic_state_t state;
  int rc = circ_element_config(( m_mdel, ELM_CONFIG_GET, /*state*/0, (int*)&state )); UPDATE_RC(rc);

  if( m_state != state )
    {
      m_state = state;
      deviceGraph->begin( BITMAP_FORMAT_RGB32 );

      switch( m_state )
      {
        case SIG_FALL:
        case SIG_LOW:
          deviceGraph->fill( 0, 0, 255 );
          deviceGraph->text( "0", READOUT_LABEL_X, READOUT_LABEL_Y, 255, 255, 255, READOUT_LABEL_SIZE, /*bold*/true );
          break;
        case SIG_RISE:
        case SIG_HIGH:
          deviceGraph->fill( 255, 0, 0 );
          deviceGraph->text( "1", READOUT_LABEL_X, READOUT_LABEL_Y, 255, 255, 255, READOUT_LABEL_SIZE, /*bold*/true );
          break;
        case SIG_FLOAT:
          deviceGraph->fill( 128, 128, 128 );
          deviceGraph->text( "X", READOUT_LABEL_X, READOUT_LABEL_Y, 255, 255, 255, READOUT_LABEL_SIZE, /*bold*/true );
          break;
      }

      deviceGraph->end();
    }

  return 0;
}

int dev_drvsrc::render_event( ISchematic *schematic, IDeviceGraph *deviceGraph, render_event_t event, int x, int y, long flags )
{
  logic_state_t state;
  int rc = circ_element_config(( m_mdel, ELM_CONFIG_GET, /*state*/0, (int*)&state )); UPDATE_RC(rc);
  switch( event )
  {
    case RENDER_EVENT_MOUSE_RELEASE:
      switch( state )
      {
        case SIG_FLOAT:
        case SIG_FALL:
        case SIG_LOW: state = SIG_HIGH; break;
        case SIG_RISE:
        case SIG_HIGH: state = SIG_LOW; break;
      }
      rc = circ_element_config(( m_mdel, ELM_CONFIG_SET, /*state*/0, (int)state ));
      break;
    default:
      break;
  }

  UNUSED(schematic); UNUSED(deviceGraph); UNUSED(x); UNUSED(y); UNUSED(flags);
  return rc;
}
