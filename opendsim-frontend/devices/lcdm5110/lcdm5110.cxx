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

#include "lcdm5110.h"

DeviceLibraryEntry *dev_lcdm5110::libraryEntry()
{
  return new DeviceLibraryEntry(
      /* symbol_name */     "LCDM5110",
      /* reference_name */  "M?",
      /* category */        "LCD Modules",
      /* sub_category */    "Nokia",
      /* description */     "Nokia 5110 LCD Module (PCD8544 84x48)",
      /* manufacturer */    "Nokia",
      /* symbolfile */      "lcdm5110.ssym",
      dev_lcdm5110::construct,
      0l);
}

IDevice *dev_lcdm5110::construct( const char *reference, int id, void *reserved )
{
  return new dev_lcdm5110( reference, id, reserved );
}

dev_lcdm5110::dev_lcdm5110( const char *reference, int id, void *reserved )
  : IDevice( reference, id, reserved )
  , m_gdram( 0l )
{}

int dev_lcdm5110::create( ISchematic *schematic, IPropertyContainer *properties )
{
  int rc = IDevice::baseinit( "LCD8544", m_circuit );               UPDATE_RC(rc);

  rc = circ_element_config(( m_mdel, ELM_CONFIG_GET, 0/*gdram*/, &m_gdram )); UPDATE_RC(rc);

  rc = properties->readDevice( this );                              UPDATE_RC(rc);

  UNUSED(schematic);
  return rc;
}

int dev_lcdm5110::init( ISchematic *schematic, IPropertyContainer *properties )
{
  schematic->registerRender( this );
  return properties->configDevice( this );
}

int dev_lcdm5110::config( int op, ... )
{
  int rc = 0;
  va_list vlist;
  va_start( vlist, op );

  switch( op )
  {
    case ELM_CONFIG_LIST_COUNT: /* Query the number of parameters */
      {
        int *count = va_arg( vlist, int* );
        *count = 0;
      }
      break;

    default:
      rc = -DS_UNIMPLEMENTED;
  }

  va_end( vlist );
  return rc;
}

int dev_lcdm5110::render_frame( ISchematic *, IDeviceGraph *deviceGraph )
{
  if( deviceGraph )
    {
      int rc;
      int PD, D, E;

      rc = circ_element_config(( m_mdel, ELM_CONFIG_GET, 1/*pd*/, &PD )); UPDATE_RC(rc);
      rc = circ_element_config(( m_mdel, ELM_CONFIG_GET, 2/*disp*/, &D )); UPDATE_RC(rc);
      rc = circ_element_config(( m_mdel, ELM_CONFIG_GET, 3/*en*/, &E )); UPDATE_RC(rc);

      deviceGraph->begin();

      if     ( PD )       deviceGraph->fill( 0, 0, 0 ); /* Power-Down mode */
      else if( !D && !E ) deviceGraph->fill( 128, 128, 128 ); /* Blank Display mode, blank the visuals */
      else if( !D && E )  deviceGraph->fill( 255, 255, 255 ); /* All segments on */
      else
        {
          for(int row=0;row<6;row++)
            {
              for( int col=0;col<84;col++ )
                {
                  char abyte = (*m_gdram)[row][col];
                  for( int bit=0; bit<8; bit++ )
                    {
                      /* This takes inverse video mode into account: */
                      deviceGraph->setPixel( col, row*8+bit, (abyte & 1) ^ ((D && E) ? 1 : 0) );
                      abyte >>= 1;
                    }
                }
            }
        }

      deviceGraph->end();
    }
  return 0;
}
