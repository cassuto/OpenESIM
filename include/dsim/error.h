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

#define DS_SUCCEEDED (0)
#define DS_FAULT (1)
#define DS_TIMEOUT (2)
#define DS_ABORTED (3)
#define DS_NO_MEMORY (4)
#define DS_WRONG_FORMAT (5)
#define DS_OVERFLOW (6)
#define DS_UNIMPLEMENTED (7)
#define DS_EOF (8)
#define DS_DUP_MODEL_SYMBOL (9)
#define DS_READONLY (10)
#define DS_SOLVE_MATRIX (11)
#define DS_WRITE_FILE (12)
#define DS_READ_FILE (13)
#define DS_CREATE_FILE (14)
#define DS_PARSE_SCHEMA (15)
#define DS_INVALID_FILE_TYPE (16)
#define DS_TYPE_MISMATCH (17)
#define DS_INVALID_ELEMENT_ID (18)
#define DS_SOLVE_SUBELEMENTS (19)

#define UPDATE_RC(_rc_) do { if( _rc_ ) return _rc_; } while(0)

#endif //!defined(DSIM_ERROR_H_)
