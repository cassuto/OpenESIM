/** @file
 * Lisp - source parser
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

#define TRACE_UNIT "lisp-parser"

#include <dsim/logtrace.h>
#include <dsim/misc.h>
#include <dsim/memory.h>
#include <dsim/scheme-intr.h>
#include <dsim/error.h>
#include <dsim/string.h>

#include "lisp-internal.h"


int
lisp_lexer_init( ds_scheme_t *sc )
{
  memset( sc->string_pool, 0, sizeof(sc->string_pool) );
  sc->string_pool_pos = 0;
  return 0;
}

static int
lisp_isdigit( ds_scheme_t *sc, char ch )
{
  if ( ch == '-' || ch == '.' || ch == 'e' || ch == 'E' || ch == '+' )
    ch = lisp_buffer_peekc( sc );
  return isdigit( ch );
}

static int
parse_number( ds_scheme_t *sc, char ch, DS_OUT ds_scheme_synlist_t **synlist )
{
  register char ch0;
  double n = 0, sign = 1, scale = 0;
  int subscale = 0, signsubscale = 1;

#define PN_GETCH() \
    do { \
      ch = lisp_buffer_getc( sc ); \
      if ( ch == EOF ) return 1; \
    } while(0) \

  *synlist = 0;

  if ( ch == '-' )
    {
      sign = -1; PN_GETCH();    /* Has sign? */
    }
  if ( ch == '0' )
    {
      PN_GETCH();    /* is zero */
    }

  if ( ch >= '1' && ch <= '9' ) do
    {
      n = (n * 10.0) + (ch -'0');
      PN_GETCH();
    }
  while ( ch >= '0' && ch <= '9' ); /* Number? */

  ch0 = lisp_buffer_peekc( sc );

  if ( ch == '.' && ch0 >= '0' && ch0 <= '9' )
    {
      PN_GETCH();    /* Fractional part? */

      do
        {
          n = (n * 10.0) + (ch -'0'), scale--;
          PN_GETCH();
        }
      while ( ch >= '0' && ch <= '9' );
    }

  if ( ch == 'e' || ch == 'E' ) /* Exponent? */
    {
      PN_GETCH();

      if ( ch == '+' )
        {
          PN_GETCH();
        }
      else if ( ch == '-' )
        {
          signsubscale = -1; PN_GETCH();    /* With sign? */
        }

      while ( ch >= '0' && ch <= '9' )
        {
          subscale = (subscale * 10) + (ch - '0');    /* Number? */
          PN_GETCH();
        }
    }

  lisp_buffer_ungetc( sc );

  n = sign * n * pow(10.0, (scale + subscale * signsubscale)); /* number = +/- number.fraction * 10^+/- exponent */

  *synlist = lisp_synlist_create();
  if ( !*synlist )
    return fault_no_memory();

  (*synlist)->synnode.type = SCHEME_VAL_NUMBER;
  (*synlist)->synnode.val.number = n;
  return 0;
}

static int
parse_boolean( ds_scheme_t *sc, DS_OUT ds_scheme_synlist_t **synlist )
{
  char val = 0;
  register char ch = lisp_buffer_getc( sc );
  if ( ch == EOF )
    return fault_unexpected_token();
  if ( ch == 't' || ch == 'T' )
    val = 1;
  else if ( ch == 'f' || ch == 'F' )
    val = 0;
  else
    return fault_unexpected_token();

  *synlist = lisp_synlist_create();
  if ( !*synlist )
    return fault_no_memory();

  (*synlist)->synnode.type = SCHEME_VAL_BOOLEAN;
  (*synlist)->synnode.val.boolean = val;
  return 0;
}

static int
parse_time( ds_scheme_t *sc, DS_OUT ds_scheme_synlist_t **synlist )
{
  int rc = 0;
  double val = 0;
  register char ch;
  char unit[2];
  char unt = SCHEME_UNIT_NONE;
  ds_scheme_synlist_t *num = NULL;

  *synlist = NULL;

  ch = lisp_buffer_getc( sc );
  if ( ch == EOF ) return fault_unexpected_token();
  if ( (rc = parse_number( sc, ch, &num)) )
    return rc;

  unit[0] = lisp_buffer_getc( sc );
  if ( unit[0] == EOF ) return fault_unexpected_token();

  /* parse the unit part */  
  if ( unit[0] == 's')
    {
      unt = SCHEME_UNIT_S;
    }
  else if ( lisp_isdelimiter( unit[0] ) )
    {
      lisp_buffer_ungetc( sc ); /* there is no unit defined */
      unt = SCHEME_UNIT_NONE;
    }
  else
    {
      unit[1] = lisp_buffer_getc( sc );
      if ( unit[1] == EOF ) return fault_unexpected_token();

      if ( unit[1] == 's' || unit[1] == 'S' )
        {
          if ( unit[0] == 'f' || unit[0] == 'F' )
             
            unt = SCHEME_UNIT_FS;
         
          else if ( unit[0] == 'p' || unit[0] == 'P' )
             
            unt = SCHEME_UNIT_PS;
         
          else if ( unit[0] == 'n' || unit[0] == 'N' )
             
            unt = SCHEME_UNIT_NS;
         
          else if ( unit[0] == 'u' || unit[0] == 'U' )
             
            unt = SCHEME_UNIT_US;
         
          else if ( unit[0] == 'm' || unit[0] == 'M' )
            
            unt = SCHEME_UNIT_MS;

          else
            return fault_unexpected_token();
        }
      else
        return fault_unexpected_token();
    }

  num->synnode.type = SCHEME_VAL_TIME;
  num->synnode.val.time.time = val;
  num->synnode.val.time.unit = unt;
  *synlist = num;
  return 0;
}

static int
parse_string( ds_scheme_t *sc, DS_OUT ds_scheme_synlist_t **synlist )
{
  register char ch;
  char *buff = NULL;
  int buff_pos = 0, buff_len = 0;
  
  *synlist = NULL;

  for ( ;; )
    {
      ch = lisp_buffer_getc( sc );
      if ( ch == EOF )
        return lisp_fault(sc, "string truncated unexpectedly\n");
      if ( ch == '"' )
        break;
      if ( buff_pos + 2 >= buff_len )  /* Resize the buffer if needed */
        {
          char *ptr = buff;
          buff_len += LISP_CHUNK_SIZE;
          
          if ( !(buff = (char *)ds_heap_realloc( buff, buff_len )) )
            {
              ds_heap_free( ptr );
              return fault_no_memory();
            }
        }
      buff[buff_pos++] = ch;
    }
  if( buff ) buff[buff_pos++] = 0;
  else buff = ds_strdup("");

  if( NULL==buff ) return fault_no_memory();

  *synlist = lisp_synlist_create();
  if ( !*synlist )
    {
      ds_heap_free( buff );
      return fault_no_memory();
    }
  (*synlist)->synnode.type = SCHEME_VAL_STRING;
  (*synlist)->synnode.val.string = buff;
  return 0;
}

static int
parse_symbol( ds_scheme_t *sc, char c, DS_OUT ds_scheme_synlist_t **synlist )
{
  register char ch = c;

  sc->string_pool_pos = 0;
  for ( ;; )
    {
      if ( isspace( ch ) || ch == '(' || ch == ')' || ch == '\"' )
        break;
      if ( sc->string_pool_pos + 2 < (int)sizeof(sc->string_pool) )
        sc->string_pool[sc->string_pool_pos++] = ch;
      else
        {
          lisp_fault( sc, "symbol string is too long\n" );
          return -DS_NO_MEMORY;
        }
      ch = lisp_buffer_getc( sc );
      if ( ch == EOF )
        return lisp_fault(sc, "string truncation\n");
    }
  lisp_buffer_ungetc( sc );

  sc->string_pool[sc->string_pool_pos++] = 0;

  *synlist = lisp_vallist_create();
  if ( !*synlist ) return fault_no_memory();

  (*synlist)->synnode.type = SCHEME_VAL_SYMBOL;
  (*synlist)->synnode.val.symbol = ds_strdup( sc->string_pool );

  if ( !(*synlist)->synnode.val.symbol )
    {
      ds_heap_free( *synlist );
      return fault_no_memory();
    }
  return 0;
}

int
lisp_lex( ds_scheme_t *sc, int dep, DS_OUT ds_scheme_synlist_t **synout )
{
  int rc = 0;
  register char ch;

  ds_scheme_synlist_t *synlist = NULL;
  ds_scheme_synlist_t *synroot = NULL, *syntail = NULL;

  *synout = NULL;

  for ( ;; )
    {
      while( isspace( ch = lisp_buffer_getc( sc ) ));
      if ( ch == EOF )
        {
          rc = -DS_EOF; break;
        }
      /* parse the string pair */
      if ( ch == '"')
        {
          if ( (rc = parse_string( sc, &synlist )) ) return rc;
        }
      /* parse the number */
      else if ( lisp_isdigit( sc, ch ) )
        {
          if ( (rc = parse_number( sc, ch, &synlist )) ) return rc;
        }
      /* parse the boolean */
      else if ( ch == '#' )
        {
          if ( (rc = parse_boolean( sc, &synlist )) ) return rc;
        }
      /* parse the time */
      else if ( ch == '@' )
        {
          if ( (rc = parse_time( sc, &synlist )) ) return rc;
        }
      /* parse the symbol */
      else if ( lisp_issymbol( ch ) )
        {
          if ( (rc = parse_symbol( sc, ch, &synlist )) ) return rc;
        }
      /* parse the pair */
      else if ( ch == '(' )
        {
          rc = lisp_lex( sc, dep+1, &synlist );
          if( rc == -DS_EOF ) break; else if( rc )  return rc;
        }
      else if ( ch == ')' )
        break;
      /* invalid token */
      else
        {
          trace_info(("ch = %d\n", ch));
          return fault_unexpected_token();
        }
      if ( LIKELY( syntail ) )
        syntail->next = synlist;
      else
        synroot = synlist;
      syntail = synlist;
    }

  *synout = lisp_vallist_create();
  if ( !*synout ) return fault_no_memory();

  (*synout)->synnode.type = SCHEME_VAL_PAIR;
  (*synout)->child = synroot;

  return rc;
}
