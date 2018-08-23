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

#ifndef DSIM_ERROR_H_
#define DSIM_ERROR_H_

#include <dsim/cdecl.h>

C_DECLS

/*
 * The following Macro definitions participate in automatic generation of error
 * messages data ( errors-generated.h ), which begins with gen\{\{ and ends with \}\}gen
 * comment.
 */

/* gen{{ */

/** Operation has been successfully completed */
#define DS_SUCCEEDED (0)
/** Operation has been failed */
#define DS_FAULT (1)
/** Operation is time-out */
#define DS_TIMEOUT (2)
/** Operation has been aborted */
#define DS_ABORTED (3)
/** Out of memory */
#define DS_NO_MEMORY (4)
/** Unexpected token */
#define DS_UNEXPECTED_TOKEN (5)
/** Wrong format */
#define DS_WRONG_FORMAT (6)
/** Buffer/Number Overflow */
#define DS_OVERFLOW (7)
/** Interface unimplemented */
#define DS_UNIMPLEMENTED (8)
/** End Of File */
#define DS_EOF (9)
/** Duplicated model symbol name */
#define DS_DUP_MODEL_SYMBOL (10)
/** Read only (can't be written) */
#define DS_READONLY (11)
/** Failed to solve the matrix */
#define DS_SOLVE_MATRIX (12)
/** Failed to write target file */
#define DS_WRITE_FILE (13)
/** Failed to read from file */
#define DS_READ_FILE (14)
/** Failed to create new file */
#define DS_CREATE_FILE (15)
/** Failed to open target file */
#define DS_OPEN_FILE (16)
/** Failed to parse schematic */
#define DS_PARSE_SCHEMA (17)
/** Invalid file type */
#define DS_INVALID_FILE_TYPE (18)
/** (scheme-lisp) Type mismatch */
#define DS_TYPE_MISMATCH (19)
/** Not matched */
#define DS_NOT_MATCHED (26)
/** Failed to create model */
#define DS_CREATE_MODEL (27)
/** (scheme-lisp) Expected a parameter but there is no */
#define DS_EXPECT_PARAM (28)

/* }}gen */

#define DS_FRONTEND (65535)


#define UPDATE_RC(_rc_) do { if( _rc_ ) return _rc_; } while(0)

typedef struct ds_error_info_s
{
  /** Pointer to the short message string. */
  const char *msgShort;
  /** Pointer to the full message string. */
  const char *msgFull;
  /** Pointer to the define string. */
  const char *msgDefine;
  /** Status code number. */
  int        code;
} ds_error_info_t;

const ds_error_info_t *ds_get_error( int rc );

END_C_DECLS

#endif //!defined(DSIM_ERROR_H_)
