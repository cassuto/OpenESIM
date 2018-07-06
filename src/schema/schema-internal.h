/*
 *  OpenDSIM (A/D mixed circuit simulator)
 *  Copyleft (C) 2016, The first Middle School in Yongsheng Lijiang China
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

#ifndef SCHEMA_INTERNAL_H_
#define SCHEMA_INTERNAL_H_

#include <dsim/misc.h>
#include <dsim/error.h>
#include <dsim/scheme-intr.h>

#include "schema-parser.h"

namespace dsim
{

enum
{
  err_sch = 1,
  err_sch_syntax,
  err_sch_invalid_format,
  err_sch_invalid_device_symbol,
  err_sch_invalid_element_symbol
};

/* schema-throw.cxx */
int schema_throw_( int errcode, const char *srcfile, int line );

#define schema_throw( rc ) schema_throw_( rc, DS_CURRENT_FILE, DS_CURRENT_LINE )

/* schema-op.cxx */
int sch_op_check_format( ds_scheme_t *sc, ds_scheme_synlist_t *in, DS_OUT ds_scheme_vallist_t **out );
int sch_op_add_device( ds_scheme_t *sc, ds_scheme_synlist_t *in, DS_OUT ds_scheme_vallist_t **out );
int sch_op_add_do( ds_scheme_t *sc, ds_scheme_synlist_t *in, DS_OUT ds_scheme_vallist_t **out );

}

#endif //!defined(SCHEMA_INTERNAL_H_)
