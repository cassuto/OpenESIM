/** @file
 * Lisp - source parser
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

#define TRACE_UNIT "lisp-parser"

#include <dsim/logtrace.h>
#include <dsim/misc.h>
#include <dsim/memory.h>
#include <dsim/scheme-intr.h>
#include <dsim/error.h>

#include "lisp-internal.h"

////////////////////////////////////////////////////////////////////////////////

int
lisp_parse( ds_scheme_t *sc, const ds_scheme_synlist_t *synlist, DS_OUT ds_scheme_vallist_t **out )
{
  int rc = 0;

  if ( synlist->synnode.type != SCHEME_VAL_PAIR )
    return -DS_FAULT;

  synlist = synlist->child;

  switch ( synlist->synnode.type )
  {
    case SCHEME_VAL_PAIR:
      if ( synlist->child )
        {
          switch ( synlist->child->synnode.type )
          {
            case SCHEME_VAL_SYMBOL: /* a pair with symbol */
              {
                ds_scheme_symbol_t *symbol = NULL;

                if ( lookup_for_symbol( sc, lispval_symbol( synlist->child ), &symbol ) )
                  {
                    return lisp_fault( sc, "unknown symbol '%s'\n", lispval_symbol( synlist->child ) );
                  }
                else
                  {
                    if ( (rc = symbol->pfn_handle( sc, synlist->child->next, out )) )
                      return rc;
                  }
              }
              break;

            default:
              *out = (ds_scheme_vallist_t*)synlist;
          }
        }
      break;

    case SCHEME_VAL_NUMBER:
    case SCHEME_VAL_BOOLEAN:
    case SCHEME_VAL_TIME:
    case SCHEME_VAL_STRING:
    case SCHEME_VAL_SYMBOL:
      *out = (ds_scheme_vallist_t*)synlist;
      break;
      
    default:
      return lisp_fault( sc, "unknown symbol type\n" );
  }

  return rc;
}

int
lisp_get_param_count( ds_scheme_t *sc, const ds_scheme_synlist_t *synlist )
{
  register int count = 0;
  const ds_scheme_synlist_t *syn = synlist;
  while ( syn )
    {
      count++;
      syn = syn->next;
    }
  return count;
}
