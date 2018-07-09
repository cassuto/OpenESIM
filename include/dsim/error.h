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
#define DS_UNEXPECTED_TOKEN (5)
#define DS_WRONG_FORMAT (6)
#define DS_OVERFLOW (7)
#define DS_UNIMPLEMENTED (8)
#define DS_EOF (9)
#define DS_DUP_MODEL_SYMBOL (10)
#define DS_READONLY (11)
#define DS_SOLVE_MATRIX (12)
#define DS_WRITE_FILE (13)
#define DS_READ_FILE (14)
#define DS_CREATE_FILE (15)
#define DS_OPEN_FILE (16)
#define DS_PARSE_SCHEMA (17)
#define DS_INVALID_FILE_TYPE (18)
#define DS_TYPE_MISMATCH (19)
#define DS_INVALID_ELEMENT_ID (20)
#define DS_INVALID_ELEMENT_SYMBOL (21)
#define DS_INVALID_DEVICE_SYMBOL (22)
#define DS_SOLVE_SUBELEMENTS (23)
#define DS_REREFERENCE (24)
#define DS_DUP_CENTRAL_ELEMENT (25)
#define DS_NOT_MATCHED (26)

#define UPDATE_RC(_rc_) do { if( _rc_ ) return _rc_; } while(0)

#endif //!defined(DSIM_ERROR_H_)
