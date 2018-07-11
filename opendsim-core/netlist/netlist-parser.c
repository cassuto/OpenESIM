/** @file
 * Netlist-Parser - top-level design
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

#include <stdio.h>

#define TRACE_UNIT "nlst"

#include <dsim/logtrace.h>
#include <dsim/misc.h>
#include <dsim/memory.h>
#include <dsim/scheme-intr.h>
#include <dsim/error.h>
#include <dsim/string.h>

#include "netlist-internal.h"

#include <dsim/netlist-parser.h>


ds_netlist_parser_t *
netlist_parser_create( ds_scheme_t *sc )
{
  ds_netlist_parser_t *nlst_parser = (ds_netlist_parser_t *)ds_heap_alloc(sizeof(*nlst_parser));
  if ( !nlst_parser ) return NULL;

  memset( nlst_parser, 0, sizeof(*nlst_parser) );

  lisp_set_opaque( sc, nlst_parser );
  nlst_parser->sc = sc;
  nlst_parser->began = false;

  return nlst_parser;
}

void
netlist_parser_release( ds_netlist_parser_t *nlst )
{
  if( nlst )
    {
      ds_heap_free( nlst );
    }
}
