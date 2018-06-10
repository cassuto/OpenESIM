/** @file
 * String operation
 */

/*
 *  OpenDSIM (Opensource Digital Circuit Simulation)
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

#include <dsim/memory.h>

#include <dsim/string.h>

////////////////////////////////////////////////////////////////////////////////

char *
ds_strdup(const char *src)
{
  char *buf = 0;
  if (src)
    {
      buf = (char *)ds_heap_alloc ( strlen ( src ) + 1 );
      if (buf)
        strcpy ( buf, src );
    }

  return buf;
}

int
ds_strcasecmp( const char *s1, const char *s2 )
{
  while  (toupper((unsigned char)*s1) == toupper((unsigned char)*s2++))
    if (*s1++ == '\0') return 0;
  return(toupper((unsigned char)*s1) - toupper((unsigned char)*--s2));
}

int
ds_strncasecmp( const char *s1, const char *s2, register int n )
{
  while (--n >= 0 && toupper((unsigned char)*s1) == toupper((unsigned char)*s2++))
    if (*s1++ == '\0')  return 0;
  return(n < 0 ? 0 : toupper((unsigned char)*s1) - toupper((unsigned char)*--s2));
}
