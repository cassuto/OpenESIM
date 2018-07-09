/** @file
 * Netlist-Parser - operation callbacks
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

#define TRACE_UNIT "nlst-op"

#include <dsim/logtrace.h>
#include <dsim/misc.h>
#include <dsim/memory.h>
#include <dsim/error.h>
#include <dsim/string.h>

#include "netlist-internal.h"

#include <dsim/netlist-parser.h>


#define GET_NLST(_sc, _nlst) ds_netlist_parser_t *_nlst = (ds_netlist_parser_t*)lisp_get_opaque( _sc )

int
nlst_op_netlist( ds_scheme_t *sc, ds_scheme_synlist_t *in, DS_OUT ds_scheme_vallist_t **out )
{
  GET_NLST( sc, nlst );

  if ( synlst_check_type_ptr( in, SCHEME_VAL_NUMBER ) )
    {
      /* validate the version number of netlist file */
      if ( lispval_number(in) > NETLIST_VERSION )
        return netlist_throw( err_nlst_version_mismatch );
    }
  else
    return netlist_throw();

  nlst->began = true;

  return 0;
}

int
nlst_op_timescale( ds_scheme_t *sc, ds_scheme_synlist_t *in, DS_OUT ds_scheme_vallist_t **out )
{
  GET_NLST( sc, nlst );

  if ( synlst_check_type_ptr( in, SCHEME_VAL_TIME ) )
    {
      nlst->timescale = lispval_time( in );
    }
  else
    return netlist_throw( err_nlst_expected_a_time );

  in = in->next;

  if ( !synlst_check_type_ptr( in, SCHEME_VAL_SYMBOL ) )
    return netlist_throw( err_nlst_invalid_token );

  in = in->next;

  if ( synlst_check_type_ptr( in, SCHEME_VAL_TIME ) )
    {
      nlst->timescale_pr = lispval_time( in );
    }
  else
    return netlist_throw( err_nlst_invalid_token );

  return 0;
}
