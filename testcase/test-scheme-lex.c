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

#include <stdlib.h>
#include <stdio.h>

#define TRACE_UNIT "tsf"

#include <dsim/logtrace.h>
#include <dsim/string.h>
#include <dsim/error.h>

#include "../src/scheme-intr/lisp-internal.h"

#include <dsim/scheme-intr.h>

#include "tsf/test.h"

////////////////////////////////////////////////////////////////////////////////

static int
scheme_read( ds_scheme_t *sc, char *buff, int maxsize )
{
  FILE *finput = (FILE*)lisp_get_stream_opaque( sc );
  int len = fread( buff, 1, maxsize, finput );
  return len > 0 ? len : -1;
}

static void
test_scheme_lexing( const char *filename )
{
  FILE *finput;
  ds_scheme_synlist_t *synlist = NULL;
  int rc = 0;
  
  trace_info((" Lexing %s\n", filename));

  finput = fopen( filename, "r" );
  ds_test_check ( NULL==finput, "open inputfile" );

  ds_scheme_t *sc = lisp_create( scheme_read, finput );
  ds_test_check ( NULL==sc, "open inputfile" );
  
  for ( ;; )
    {
      rc = lisp_lex( sc, &synlist );
      if ( rc == -DS_EOF )
        break;
      else
        ds_test_check ( rc, "lisp_lex()" );
    }
    
  fclose( finput );
  lisp_release( sc );
}

int
main( int argc, char *argv[] )
{
  if ( ds_test_init( "test-scheme-lex" ) ) return 1;

  test_scheme_lexing( "test-scheme.nlst" );
  
  return 0;
}
