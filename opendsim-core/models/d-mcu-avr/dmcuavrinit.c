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

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dmcuavr.h"

typedef enum
{
  FIRMWARE_UNKNOWN = 0,
  FIRMWARE_HEX,
  FIRMWARE_ELF
} firmware_type_t;

static inline firmware_type_t
get_firmware_type( const char *fn )
{
  const char *ext_offset = 0l;
  size_t len = strlen( fn );
  fn += len;
  while( len-- )
    {
      if( *(--fn) == '.' )
        {
          ext_offset = fn;
          break;
        }
    }
  if( ext_offset )
    {
      if( 0 == strcmp( ext_offset, ".hex" ) )
        return FIRMWARE_HEX;
      else if( 0 == strcmp( ext_offset, ".elf" ) )
        return FIRMWARE_ELF;
    }
  return FIRMWARE_UNKNOWN;
}

static inline int
fixup_addr( int addr )
{
  if( addr < 64 ) addr += 32;
  return addr;
}

static int
get_reg_addr( const regmap_t *map, const char *entry )
{
  for( int i=0; map[i].entry; i++ )
    {
      if( 0==strcmp( map[i].entry, entry ) )
        {
          return fixup_addr( map[i].address );
        }
    }
  return -1;
}

/*
 * Callback to process IRQs
 */
static void
uart_pty_out_cb( struct avr_irq_t* irq, uint32_t value, void* pparam )
{
  UNUSED( irq );
  circ_element_t* element = (circ_element_t *)pparam;
  DEFINE_PARAM(param, element, mcu_avr_param_t);

  mdel_logtrace( MDEL_INFO, ("UART: out %d (%c)", value, value) );

  UNUSED( param ); //!todo processing usart output
  UNUSED( value );
}

static void
port_cb( struct avr_irq_t *irq, uint32_t value, void *pparam )
{
  UNUSED(irq);
  circ_element_t* element = (circ_element_t *)pparam;
  DEFINE_PARAM(param, element, mcu_avr_param_t);
  UNUSED( param );
  UNUSED( value );
}

static void
port_reg_cb( struct avr_irq_t *irq, uint32_t value, void *pparam )
{
  UNUSED(irq);
  circ_element_t* element = (circ_element_t *)pparam;
  DEFINE_PARAM(param, element, mcu_avr_param_t);
  UNUSED( param );
  UNUSED( value );
}

static void
ddr_cb( struct avr_irq_t *irq, uint32_t value, void *pparam )
{
  UNUSED(irq);
  circ_element_t* element = (circ_element_t *)pparam;
  DEFINE_PARAM(param, element, mcu_avr_param_t);
  UNUSED( param );
  UNUSED( value );
}

extern int
elf_read_firmware_ext(const char * file, elf_firmware_t * firmware); // thirdparty/sim/read_elf.c

static int
mcu_avr_map_pins( avr_t *avr_processor, const regmap_t *regmap, const pinmap_t *pinmap, mcu_avr_param_t *param, circ_element_t *element )
{
  for( int i=0; pinmap[i].typemask != PIN_END; i++ )
    {
      int pinid = i;

      if( pinmap[i].bank != 'X' )
      {
        char pinreg[8];

        /*
         * PORTX Register IRQs
         */
        strcpy( pinreg, "PORT" );
        pinreg[4] = pinmap[i].bank;
        pinreg[5] = '\0';
        int port_addr = get_reg_addr( regmap, pinreg );
        if( port_addr < 0 )
          {
            mdel_logtrace( MDEL_ERR, ("Unknown port register: \"%s\"!", pinreg) );
            return -DS_FAULT;
          }
        param->port_irqs[pinid] = avr_iomem_getirq( avr_processor,
                              port_addr,
                              &(pinmap[i].bank),
                              pinmap[i].bankid );

        avr_irq_register_notify( param->port_irqs[pinid], port_reg_cb, element );

        param->port_irqs[pinid] = avr_io_getirq( avr_processor, AVR_IOCTL_IOPORT_GETIRQ( pinmap[i].bank ), pinmap[i].bankid );
        avr_irq_register_notify( param->port_irqs[pinid], port_cb, element );

        /*
         * DDRX Register IRQs
         */
        strcpy( pinreg, "DDR" );
        pinreg[3] = pinmap[i].bank;
        pinreg[4] = '\0';
        int ddr_addr = get_reg_addr( regmap, pinreg );
        if( ddr_addr < 0 )
          {
            mdel_logtrace( MDEL_ERR, ("Unknown ddr register: \"%s\"!", pinreg) );
            return -DS_FAULT;
          }

        param->ddr_irqs[pinid] = avr_iomem_getirq( avr_processor,
                                                   ddr_addr,
                                                   " ",
                                                   pinmap[i].bankid );

        avr_irq_register_notify( param->ddr_irqs[pinid], ddr_cb, element );

        /*
         * IRQ for writing ports
         */
        param->wr_port_irqs[pinid] = avr_io_getirq( avr_processor, AVR_IOCTL_IOPORT_GETIRQ(pinmap[i].bank), pinmap[i].bankid );

      }
    }
  return 0; // succeeded
}

static void
avr_logger( struct avr_t* avr, const int level, const char * format, va_list ap )
{
  char buff[2048];
  vsnprintf( buff, sizeof(buff), format, ap );

  int mdel_level;
  switch( level )
  {
    case LOG_OUTPUT: mdel_level = MDEL_INFO; break;
    case LOG_ERROR: mdel_level = MDEL_ERR; break;
    case LOG_WARNING: mdel_level = MDEL_WARNING; break;
    case LOG_TRACE: mdel_level = MDEL_VERBOSE; break;
    case LOG_DEBUG: mdel_level = MDEL_VERBOSE; break;
    case LOG_NONE:
    default:
      mdel_level = MDEL_INFO;
  }

  size_t len = strlen(buff);
  char *pos = buff + len - 1;
  if( *pos == '\n' ) *pos = '\0';
  mdel_logtrace( mdel_level, ("%s", buff ) );
}

int
LIB_FUNC(mcu_avr_init)( circ_element_t *element )
{
  int rc;
  DEFINE_PARAM(param, element, mcu_avr_param_t);

  avr_global_logger_set( avr_logger );

  if( param->romimg_fn )
    {
      elf_firmware_t elf;
      memset( &elf, 0, sizeof(elf) );

      /*
       * Read the binary image and fill in buffer of flash and eeprom
       */
      switch( get_firmware_type( param->romimg_fn ) )
      {
        case FIRMWARE_HEX:
          {
            ihex_chunk_p chunk = NULL;
            int cnt = read_ihex_chunks( param->romimg_fn, &chunk );

            if( cnt <= 0 )
              {
                mdel_logtrace( MDEL_ERR, ("Unable to load .hex file \"%s\"!", param->romimg_fn) );
                return -DS_OPEN_FILE;
              }

            int lastFChunk = 0;

            for( int ci = 0; ci < cnt; ci++ )
              {
                if( chunk[ci].baseaddr < (1*1024*1024) ) lastFChunk = ci;
              }
            elf.flashbase = chunk[ 0 ].baseaddr;
            elf.flashsize = chunk[ lastFChunk ].baseaddr + chunk[ lastFChunk ].size;
            elf.flash = (uint8_t*) malloc( elf.flashsize+1 );

            for( int ci = 0; ci < cnt; ci++ )
              {
                if( chunk[ci].baseaddr < (1*1024*1024) )
                  {
                    memcpy( elf.flash + chunk[ci].baseaddr,
                            chunk[ci].data,
                            chunk[ci].size );
                  }
                if( chunk[ci].baseaddr >= AVR_SEGMENT_OFFSET_EEPROM )
                  {
                    elf.eeprom = chunk[ci].data;
                    elf.eesize = chunk[ci].size;
                  }
              }
            break;
          }

        case FIRMWARE_ELF:
          {
            elf.flashsize = 0;
            elf_read_firmware_ext( param->romimg_fn, &elf );
            if (!elf.flashsize)
              {
                mdel_logtrace( MDEL_ERR, ("Unable to load .elf file \"%s\"!", param->romimg_fn) );
                return -DS_OPEN_FILE;
              }
            break;
          }

        default:
          {
            mdel_logtrace( MDEL_ERR, ("Unknown file format for ROM image (must be suffixed .hex or .elf): \"%s\"!", param->romimg_fn) );
            return -DS_OPEN_FILE;
          }
      }

      /*
       * Validate the MCU model name specified by elf structure if needed, or copy specified model for simavr
       */
      if( !param->mmcu || param->mmcu[0] == '\0' )
        {
          mdel_logtrace( MDEL_ERR, ("MCU model should be specified!") );
          return -DS_FAULT;
        }
      if( elf.mmcu[0] )
        {
          if( strcmp( elf.mmcu, param->mmcu ) )
            {
              mdel_logtrace( MDEL_WARNING, ("Incompatible rom image: current processor=\"%s\", but compiled for \"%s!\"", elf.mmcu, param->mmcu) );
            }
        }
      else
        {
          strcpy( elf.mmcu, param->mmcu );
        }

      /*
       * Create instance of processor.
       */
      if( !param->avr_processor )
        {
          param->avr_processor = avr_make_mcu_by_name( elf.mmcu );

          if( !param->avr_processor )
            {
              mdel_logtrace( MDEL_ERR, ("Failed to create MCU model \"%s\"", elf.mmcu ) );
              return -DS_FAULT;
            }
          if( (rc = avr_init( param->avr_processor )) )
            return rc;

          /*
           * Configuration device tree of USART Interface
           */

          /* Irq to send data to terminal panel */
          avr_irq_t* src = avr_io_getirq( param->avr_processor , AVR_IOCTL_UART_GETIRQ('0'), UART_IRQ_OUTPUT );
          avr_irq_register_notify( src, uart_pty_out_cb, element );

          /* Irq to send data to AVR: */
          param->uart_in_irq = avr_io_getirq( param->avr_processor, AVR_IOCTL_UART_GETIRQ('0'), UART_IRQ_INPUT );

          mdel_logtrace( MDEL_INFO, ("MCU ROM image loaded with model \"%s\"", elf.mmcu) );
        }
      /* fixme application will crash on the invalid firmware load */
      avr_load_firmware( param->avr_processor, &elf );

      if( elf.flashbase ) param->avr_processor->pc = elf.flashbase;

      param->avr_processor->frequency = 16000000;
      param->avr_processor->cycle = 0;

      param->mcu = get_mmcu( elf.mmcu );
      if( param->mcu )
        {
          int pincount = param->mcu->pincount;
          param->state = ds_heap_alloc( sizeof(logic_state_t) * pincount );
          param->port_irqs = ds_heap_alloc( sizeof(avr_irq_t *) * pincount );
          param->ddr_irqs = ds_heap_alloc( sizeof(avr_irq_t *) * pincount );
          param->wr_port_irqs = ds_heap_alloc( sizeof(avr_irq_t *) * pincount );
          if( param->state && param->port_irqs && param->ddr_irqs && param->wr_port_irqs )
            {
              if( (rc = mcu_avr_map_pins( param->avr_processor, param->mcu->regmap, param->mcu->pinmap, param, element )) )
                return rc;

              for( int i=0; i < pincount; i++ )
                {
                  if( (param->mcu->pinmap[i].typemask & PIN_VCC) ||
                      (param->mcu->pinmap[i].typemask & PIN_GND) ||
                      (param->mcu->pinmap[i].typemask & PIN_AREF) ||
                      (param->mcu->pinmap[i].typemask & PIN_AVCC) )
                    {
                      if( element->pin_vector[i]->connected )
                        if( (rc = circ_node_add_changed_fast( PINNODE(element,i), element )) ) /* analog field */
                          return rc;
                    }
                  else if( param->mcu->pinmap[i].typemask & PIN_IO )
                    {
                      if( element->pin_vector[i]->connected )
                        if( (rc = circ_node_add_logic( PINNODE(element, i), element )) ) /* digital field */
                          return rc;
                    }
                }

              return 0; // succeeded
            }
          else
            return -DS_NO_MEMORY;
        }
      else
        {
          mdel_logtrace( MDEL_ERR, ("Register and pin mappings for processor \"%s\" is not found!", elf.mmcu) );
          return -DS_FAULT;
        }
    }
  else
    {
      mdel_logtrace(MDEL_ERR, ( "No ROM image file specified!" ));
      return -DS_FAULT;
    }

  return -DS_NO_MEMORY;
}
