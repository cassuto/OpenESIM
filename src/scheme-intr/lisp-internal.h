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

#ifndef LISP_INTERNAL_H_
#define LISP_INTERNAL_H_

#include <stdio.h>
#include <string.h>
#include <math.h>

#include <dsim/misc.h>
#include <dsim/scheme-intr.h>
#include <dsim/string.h>

/* lisp-buffer.c */
int lisp_buffer_init( ds_scheme_t *sc, ds_scheme_pfn_read stream_in_read, void *stream_opaque );
char lisp_buffer_getc( ds_scheme_t *sc );
void lisp_buffer_ungetc( ds_scheme_t *sc );
char lisp_buffer_peekc( ds_scheme_t *sc );

/* lisp-fault.c */
int lisp_fault( ds_scheme_t *sc, const char *msg, ... );

/* lisp-lexer.c */
int lisp_lexer_init( ds_scheme_t *sc );
int lisp_lex( ds_scheme_t *sc, int dep, DS_OUT ds_scheme_synlist_t **synout );

#define fault_no_memory()           (lisp_fault(sc, "no memory\n"), -DS_NO_MEMORY)
#define fault_unexpected_token()    (lisp_fault(sc, "unexpected token\n"), -DS_NO_MEMORY)

/* this mess is to tell the token by its first letter */
static inline int lisp_isdelimiter( char ch )
{
  return isspace( ch ) || ch == ')' || ch == '(' || ch == '"';
}
static inline int lisp_issymbol( char ch )
{
  return isalpha( ch ) || ch == '/';
}

/* lisp.c */
int lookup_for_symbol( ds_scheme_t *sc, const char *symbol_name, DS_OUT ds_scheme_symbol_t **symout );
void lisp_symbol_free( void *elem );

#endif //!defined(LISP_INTERNAL_H_)
