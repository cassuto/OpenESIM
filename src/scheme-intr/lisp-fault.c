/** @file
 * Lisp - fault display
 */

/*
 *  OpenDSIM (Opensource Digital Circuit Simulation)
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

#include <stdarg.h>
#include <stdio.h>

#define TRACE_UNIT "lisp"

#include <dsim/logtrace.h>
#include <dsim/misc.h>
#include <dsim/memory.h>
#include <dsim/error.h>

#include "lisp-internal.h"

////////////////////////////////////////////////////////////////////////////////

int
lisp_fault( ds_scheme_t *sc, const char *msg, ...)
{
  char buff[2048];
  va_list args;
  va_start( args, msg );

  vsnprintf( buff, sizeof(buff), msg, args );
  va_end( args );

  trace_error(("line %d: %s", sc->buffer_linenum, buff));
  return -DS_FAULT;
}
