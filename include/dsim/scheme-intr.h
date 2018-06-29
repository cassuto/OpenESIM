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

#ifndef SCHEME_INTR_H_
#define SCHEME_INTR_H_

#include <dsim/misc.h>
#include <dsim/hashmap.h>
#include <dsim/cdecl.h>

C_DECLS

/** static buffer used for source reading */
#define LISP_BUFFER_UNGET_SIZE (4)
#define LISP_BUFFER_CHUNK_SIZE (8192)
/** static string pool used for symbols */
#define LISP_STRING_POOL_SIZE (8192)
/** chunk size of per string allocating */
#define LISP_CHUNK_SIZE (1024)

#ifndef EOF
# define EOF (-1)
#endif

typedef struct ds_scheme_s ds_scheme_t;
typedef struct ds_scheme_symbol_s ds_scheme_symbol_t;
typedef struct ds_scheme_synlist_s ds_scheme_synlist_t;

#define ds_scheme_valnode_t ds_scheme_synnode_t
#define ds_scheme_vallist_t ds_scheme_synlist_t

typedef int (*ds_scheme_pfn_read)( ds_scheme_t *sc, char *buff, int maxsize );
typedef int (*ds_scheme_pfn_symbol_handle)( ds_scheme_t *sc, ds_scheme_synlist_t *in, DS_OUT ds_scheme_vallist_t **out );

typedef struct ds_scheme_s
{
  hashmap_t symbols;

  char buffer[LISP_BUFFER_UNGET_SIZE + LISP_BUFFER_CHUNK_SIZE];
  int buffer_write_pos;
  int buffer_read_pos;
  int buffer_linenum, buffer_filepos;
  char buffer_unline;
  ds_scheme_pfn_read stream_in_read;
  void *stream_opaque;
  void *handler_opaque;

  char string_pool[LISP_STRING_POOL_SIZE];
  int string_pool_pos;
}
ds_scheme_t;

typedef enum
{
  SCHEME_VAL_INVALID = 0,
  SCHEME_VAL_NUMBER,
  SCHEME_VAL_STRING,
  SCHEME_VAL_BOOLEAN,
  SCHEME_VAL_TIME,
  SCHEME_VAL_SYMBOL,
  SCHEME_VAL_PAIR
} ds_scheme_type_t;

enum ds_scheme_timeunit
{
  SCHEME_UNIT_NONE = 0,
  SCHEME_UNIT_FS,
  SCHEME_UNIT_PS,
  SCHEME_UNIT_NS,
  SCHEME_UNIT_US,
  SCHEME_UNIT_MS,
  SCHEME_UNIT_S
};

typedef struct ds_scheme_time_s
{
  double time;
  char unit;
} ds_scheme_time_t;

typedef struct ds_scheme_synnode_s
{
  ds_scheme_type_t type;
  union
  {
    char boolean;
    char *string;
    double number;
    char *symbol;
    ds_scheme_time_t time;
  } val;
} ds_scheme_synnode_t;

typedef struct ds_scheme_synlist_s
{
  ds_scheme_synnode_t synnode;
  struct ds_scheme_synlist_s *child;
  struct ds_scheme_synlist_s *next;
} ds_scheme_synlist_t;


typedef struct ds_scheme_symbol_s
{
  DS_HASHMAP_NODE();

  char *symbol_name;
  unsigned symbol_name_len;
  ds_scheme_pfn_symbol_handle pfn_handle;
  struct ds_scheme_symbol_s *next;
} ds_scheme_symbol_t;

/* lisp.c */
ds_scheme_t *lisp_create( ds_scheme_pfn_read stream_in_read, void *stream_opaque );
int lisp_eval( ds_scheme_t *sc );
int lisp_print( ds_scheme_vallist_t *vallist, char *buff, int maxlen );
void lisp_release( ds_scheme_t *sc );
int lisp_synnode_dup( const ds_scheme_synnode_t *src, ds_scheme_synnode_t *syn);
ds_scheme_synlist_t *lisp_synlist_create( void );
void lisp_synlist_free( ds_scheme_synlist_t *synlist );
int lisp_add_symbol( ds_scheme_t *sc, const char *symbol_name, ds_scheme_pfn_symbol_handle pfn_handle);

/* lisp-parser.c */
int lisp_parse( ds_scheme_t *sc, const ds_scheme_synlist_t *synlist, DS_OUT ds_scheme_vallist_t **out );
int lisp_get_param_count( ds_scheme_t *sc, const ds_scheme_synlist_t *synlist );

#define lisp_vallist_create lisp_synlist_create
#define lisp_vallist_free lisp_synlist_free

static inline
void lisp_parse_free( const ds_scheme_synlist_t *synlist, ds_scheme_vallist_t *vallist )
{
  if ( synlist != vallist )
    lisp_vallist_free( vallist );
}

/* opaque for stream callback */
static inline void *
lisp_get_stream_opaque( ds_scheme_t *sc )
{
  return sc->stream_opaque;
}

/* opaque for handler callback */
static inline void
lisp_set_opaque( ds_scheme_t *sc, void *opaque )
{
  sc->handler_opaque = opaque;
}

static inline void *
lisp_get_opaque( ds_scheme_t *sc )
{
  return sc->handler_opaque;
}

/* some handy macros */
#define synlst_check_type_ptr(_synlist, _type) ((_synlist) && (_synlist)->synnode.type == (_type))

#define lispval_boolean(_vallist) ((_vallist)->synnode.val.boolean)
#define lispval_string(_vallist) ((_vallist)->synnode.val.string)
#define lispval_number(_vallist) ((_vallist)->synnode.val.number)
#define lispval_symbol(_vallist) ((_vallist)->synnode.val.symbol)
#define lispval_time(_vallist) ((_vallist)->synnode.val.time)

END_C_DECLS

#endif //!defined(SCHEME_INTR_H_)
