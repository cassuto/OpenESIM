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

#ifndef DSIM_STRING_H_
#define DSIM_STRING_H_

#include <dsim/cdecl.h>
#include <dsim/types.h>
#include <dsim/misc.h>

C_DECLS

char *ds_strdup(const char *src);
int ds_strcasecmp( const char *s1, const char *s2 );
int ds_strncasecmp( const char *s1, const char *s2, register int n );
int ds_atoi( const char *src, int len, int DS_OUT *val );
int ds_atof( const char *src, int len, float DS_OUT *val );
int ds_atob(const char *src, int len, bool DS_OUT *bl);
int ds_itoa( int src, int radix, char DS_OUT *buff, int len, int DS_OUT *outlen );
int ds_ftoa( float src, int radix, long precision, char DS_OUT *buff, int len, int DS_OUT *outlen );
int ds_btoa( bool src, char DS_OUT *buff, int len, int DS_OUT *outlen );

#include <string.h>
#include <ctype.h>

END_C_DECLS

#endif //!defined(DSIM_STRING_H_)
