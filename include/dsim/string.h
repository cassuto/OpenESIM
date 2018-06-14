/*
 *  OpenDSIM (Opensource Circuit Simulator)
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

char *ds_strdup(const char *src);
int ds_strcasecmp( const char *s1, const char *s2 );
int ds_strncasecmp( const char *s1, const char *s2, register int n );

#include <string.h>
#include <ctype.h>

#endif //!defined(DSIM_STRING_H_)
