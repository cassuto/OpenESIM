/** @file
 * Lisp - top-level design
 */

/*
 *  OpenDSIM (Opensource Circuit Simulator)
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

#define TRACE_UNIT "lisp"

#include <dsim/logtrace.h>
#include <dsim/misc.h>
#include <dsim/memory.h>
#include <dsim/scheme-intr.h>
#include <dsim/error.h>
#include <dsim/string.h>

#include "lisp-internal.h"

////////////////////////////////////////////////////////////////////////////////


ds_scheme_t *
lisp_create( ds_scheme_pfn_read stream_in_read, void *stream_opaque )
{
  ds_scheme_t *sc = (ds_scheme_t *)ds_heap_alloc(sizeof(*sc));
  if ( !sc ) return NULL;

  memset( sc, 0, sizeof(*sc) );

  if ( lisp_buffer_init( sc, stream_in_read, stream_opaque ) )
    goto err_out;
  if ( lisp_lexer_init( sc ) )
    goto err_out;
  if ( hashmap_init( &sc->symbols, HASHMAP_KEY_INTPTR, 64 ) )
    goto err_out;

  return sc;
err_out:
  if ( sc ) ds_heap_free( sc );
  return NULL;
}

void
lisp_release( ds_scheme_t *sc )
{
  if ( sc )
    {
      hashmap_clear( &sc->symbols, lisp_symbol_free );
      hashmap_uninit( &sc->symbols );
      ds_heap_free( sc );
    }
}

ds_scheme_synlist_t *
lisp_synlist_create( void )
{
  ds_scheme_synlist_t *synlist = (ds_scheme_synlist_t *)ds_heap_alloc(sizeof(*synlist));
  if ( !synlist ) return NULL;

  memset( synlist, 0, sizeof(*synlist) );
  return synlist;
}

int
lisp_synnode_dup( const ds_scheme_synnode_t *src, ds_scheme_synnode_t *syn)
{
  int rc = 0;

  switch ( src->type )
  {
    case SCHEME_VAL_NUMBER:
      syn->val.number = src->val.number;
      break;

    case SCHEME_VAL_STRING:
      syn->val.string = src->val.string ? ds_strdup( src->val.string ) : NULL;
      if ( !syn->val.string ) rc = -DS_NO_MEMORY;
      break;

    case SCHEME_VAL_BOOLEAN:
      syn->val.boolean = src->val.boolean;
      break;

    case SCHEME_VAL_TIME:
      syn->val.time = src->val.time;
      break;

    case SCHEME_VAL_SYMBOL:
      syn->val.symbol = src->val.symbol ? ds_strdup( src->val.symbol ) : NULL;
      if ( !syn->val.symbol ) rc = -DS_NO_MEMORY;
      break;

    case SCHEME_VAL_PAIR:
      return -DS_UNIMPLEMENTED;

    case SCHEME_VAL_INVALID:
      return -DS_FAULT;
  }

  syn->type = src->type;
  return rc;
}

void
lisp_synlist_free( ds_scheme_synlist_t *synlist )
{
  if ( synlist )
    {
      ds_scheme_synlist_t *syn = synlist, *cur = NULL;
      while( syn )
        {
          cur = syn;
          syn = syn->next;

          switch ( cur->synnode.type )
          {
            case SCHEME_VAL_STRING:
              if ( cur->synnode.val.string )
                ds_heap_free( cur->synnode.val.string );
              break;

            case SCHEME_VAL_SYMBOL:
              if ( cur->synnode.val.symbol )
                ds_heap_free( cur->synnode.val.symbol );
              break;

            default:
              break;
          }

          if ( cur->child )
            lisp_synlist_free( cur->child );

          ds_heap_free( cur );
        }
    }
}

void
lisp_symbol_free( void *elem )
{
  ds_scheme_symbol_t *symbol = (ds_scheme_symbol_t*)elem;
  if ( symbol )
    {
      if ( symbol->symbol_name ) ds_heap_free( symbol->symbol_name );
      ds_heap_free( symbol );
    }
}

int
lisp_add_symbol( ds_scheme_t *sc, const char *symbol_name, ds_scheme_pfn_symbol_handle pfn_handle)
{
  TRACE_ASSERT( symbol_name && pfn_handle );

  ds_scheme_symbol_t *symbol = (ds_scheme_symbol_t*)ds_heap_alloc(sizeof(*symbol));
  if ( !symbol )
    return fault_no_memory();

  symbol->symbol_name = ds_strdup( symbol_name );
  if ( !symbol->symbol_name )
    {
      ds_heap_free( symbol );
      return fault_no_memory();
    }
  symbol->symbol_name_len = strlen( symbol_name );
  symbol->pfn_handle = pfn_handle;

  hashmap_insert( &sc->symbols, (hashmap_key_t)(symbol->symbol_name), hashmap_node(symbol) );
  return 0;
}

int
lookup_for_symbol( ds_scheme_t *sc, const char *symbol_name, DS_OUT ds_scheme_symbol_t **symout )
{
  *symout = hashmap_entry( hashmap_at( &sc->symbols, (hashmap_key_t)symbol_name ), ds_scheme_symbol_t );
  return *symout ? 0 : -DS_FAULT;
}

int
lisp_eval( ds_scheme_t *sc )
{
  int rc = 0, eof = 0;
  ds_scheme_synlist_t *synlist = NULL;
  ds_scheme_vallist_t *vallist = NULL;

  for ( ;; )
    {
      rc = lisp_lex( sc, &synlist );
      if ( rc == -DS_EOF )
         eof = 1;
      else if ( rc )
         return rc;

      if ( (rc = lisp_parse( sc, synlist, &vallist )) ) return rc;

      lisp_synlist_free ( synlist );
      lisp_parse_free ( synlist, vallist );

      if ( eof ) break;
    }
  return 0;
}

/* will not return a negative value of current pos if the buffer is overflow  */
int
lisp_print( ds_scheme_vallist_t *vallist, char *buff, int maxlen )
{
#define CHECK_POS() do{ if( maxlen - pos <= 0 ) return pos; } while(0)
  int pos = 0;
  const char *disp;

  switch ( vallist->synnode.type )
  {
      /* print number */
    case SCHEME_VAL_NUMBER:
      CHECK_POS(); pos += snprintf( &buff[pos], maxlen - pos, "%lf", lispval_number( vallist ) );
      break;

      /* print string */
    case SCHEME_VAL_STRING:
      disp = lispval_string( vallist );
      CHECK_POS(); pos += snprintf( &buff[pos], maxlen - pos, "%s", disp ? disp : "nil" );
      break;

      /* print boolean */
    case SCHEME_VAL_BOOLEAN:
      disp = lispval_boolean( vallist ) ? "#t" : "#f";
      CHECK_POS(); pos += snprintf( &buff[pos], maxlen - pos, "%s", disp );
      break;

      /* print time */
    case SCHEME_VAL_TIME:
      switch ( lispval_time( vallist ).unit )
      {
        case SCHEME_UNIT_NONE: disp = ""; break;
        case SCHEME_UNIT_FS: disp = "fs"; break;
        case SCHEME_UNIT_PS: disp = "ps"; break;
        case SCHEME_UNIT_NS: disp = "ns"; break;
        case SCHEME_UNIT_US: disp = "us"; break;
        case SCHEME_UNIT_MS: disp = "ms"; break;
        case SCHEME_UNIT_S:  disp = "s";  break;
        default: disp = "?"; break;
      }
      CHECK_POS(); pos += snprintf( &buff[pos], maxlen - pos, "%lf %s", lispval_time( vallist ).time, disp );
      break;

      /* print symbol name */
    case SCHEME_VAL_SYMBOL:
      disp = lispval_symbol( vallist );
      CHECK_POS(); pos += snprintf( &buff[pos], maxlen - pos, "%s", disp ? disp : "nil" );
      break;

      /* print pair */
    case SCHEME_VAL_PAIR:
      {
        ds_scheme_vallist_t *node = vallist;

        CHECK_POS(); pos += snprintf( &buff[pos], maxlen - pos, "( " );
        while (node)
          {
            if (node->synnode.type == SCHEME_VAL_PAIR)
              {
                CHECK_POS(); pos += lisp_print( node->child, &buff[pos], maxlen - pos );
                CHECK_POS(); pos += snprintf( &buff[pos], maxlen - pos, " " );
              }
            else
              {
                CHECK_POS(); pos += snprintf( &buff[pos], maxlen - pos, ". " );
                CHECK_POS(); pos += lisp_print( node, &buff[pos], maxlen - pos );
                CHECK_POS(); pos += snprintf( &buff[pos], maxlen - pos, " " );
              }
            node = node->next;
          }
        CHECK_POS(); pos += snprintf( &buff[pos], maxlen - pos, ") " );
      }
      break;

    default:
      return -DS_FAULT;
  }
  return pos;
}
