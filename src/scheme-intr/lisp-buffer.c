/** @file
 * Lisp - source buffer
 */

/*
 *  OpenDSIM (A/D mixed circuit simulator)
 *  Copyright (C) 2016, The first Middle School in Yongsheng Lijiang China
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

#define TRACE_UNIT "lisp-buf"

#include <dsim/logtrace.h>
#include <dsim/misc.h>
#include <dsim/memory.h>
#include <dsim/string.h>

#include "lisp-internal.h"


int
lisp_buffer_init( ds_scheme_t *sc, ds_scheme_pfn_read stream_in_read, void *stream_opaque )
{
  sc->buffer_write_pos = 0;
  sc->buffer_read_pos = sizeof(sc->buffer);
  sc->buffer_linenum = 1;
  sc->buffer_unline = 0;
  sc->buffer_filepos = 0;
  sc->stream_in_read = stream_in_read;
  sc->stream_opaque = stream_opaque;
  return 0;
}

char
lisp_buffer_getc( ds_scheme_t *sc )
{
  register char c;
  register int len, total = 0;

  if ( sc->buffer_read_pos >= sizeof(sc->buffer) )
    {
      memmove( sc->buffer, sc->buffer + sizeof(sc->buffer) - LISP_BUFFER_UNGET_SIZE, LISP_BUFFER_UNGET_SIZE );

      /* swapping (redirecting) the pointer if needed */
      sc->buffer_read_pos = LISP_BUFFER_UNGET_SIZE;
      sc->buffer_write_pos = sc->buffer_read_pos;

      /* fill in the cache (buffer) */
      while( sc->buffer_write_pos < sizeof(sc->buffer) - LISP_BUFFER_UNGET_SIZE )
        {
          int remain = sizeof(sc->buffer) - LISP_BUFFER_UNGET_SIZE - sc->buffer_write_pos;

          len = sc->stream_in_read( sc, sc->buffer + sc->buffer_write_pos, remain );
          if ( len > 0 )
            sc->buffer_write_pos += len;
          else
            {
              /* insert EOFs */
              memset( sc->buffer + sc->buffer_write_pos, EOF, remain );
              sc->buffer_write_pos += remain;
            }
        }
    }

   c = sc->buffer[sc->buffer_read_pos];
   ++(sc->buffer_read_pos);

   /* should we take charge of the state of position here?? */
   if ( c == '\n') sc->buffer_linenum++;
   sc->buffer_unline = 0;
   sc->buffer_filepos++;

   return c;
}

void
lisp_buffer_ungetc( ds_scheme_t *sc )
{
  if ( sc->buffer_read_pos-1 <= 0 )
    {
      /*
       * As json_getc() has properly processed the pointer swapping, there is
       * no need to consider on the problem that the bound string at the end of
       * buffer would be lost. However, json_unget should be performed within the
       * maximum cancelable bytes specified by JSON_BUFFER_UNGET_SIZE, otherwise we will get an assert failure.
       */
      trace_assert(0);
      return;
    }

  /* this mess is to ensure the consistency of line number and filepos */
  if ( sc->buffer_unline )
    sc->buffer_linenum--;
  sc->buffer_unline = ( sc->buffer[sc->buffer_read_pos-1] == '\n' );
  sc->buffer_filepos--;

  --(sc->buffer_read_pos);
}

char
lisp_buffer_peekc( ds_scheme_t *sc )
{
  register char ch = lisp_buffer_getc( sc );
  lisp_buffer_ungetc( sc );
  return ch;
}
