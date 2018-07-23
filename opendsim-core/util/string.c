/** @file
 * String operation
 */

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

#include <dsim/memory.h>
#include <dsim/error.h>
#include <dsim/misc.h>

#include <dsim/string.h>

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
ds_strncasecmp( const char *s1, const char *s2, int n )
{
  while (--n >= 0 && toupper((unsigned char)*s1) == toupper((unsigned char)*s2++))
    if (*s1++ == '\0')  return 0;
  return(n < 0 ? 0 : toupper((unsigned char)*s1) - toupper((unsigned char)*--s2));
}

/**
 * Convert the string to integer.
 *
 * The process of conversation will stop if it meets
 * any non-number characters, expect '+' or '-' that can
 * sign positive and negative.
 *
 * @return status code.
 * @param src           Pointer to source string buffer.
 * @param len           Length of string.
 * @parma val           Where to store the result.
 */
int ds_atoi( const char *src, int len, int DS_OUT *val )
{
  bool minus = false;
  int res = 0;
  for (int i = 0; i < len; i++)
    {
      if (src[i] == '-')
        {
          minus = true;
          continue;
        }
      if (src[i] == ' ')
          continue;
      if ('0' > src[i] || src[i] > '9')
          break;
      else
        {
          res = res * 10 + (src[i] - '0');
          continue;
        }
    }
  *val = minus ? -res : res;
  return 0;
}

/**
 * Convert the string to float.
 *
 * The process of conversation will stop if it meets
 * any non-number characters, expect '+' or '-' that can
 * sign positive and negative.
 *
 * @return status code.
 * @param src           Pointer to source string buffer.
 * @param len           Length of string.
 * @parma val           Where to store the result.
 */
int ds_atof( const char *src, int len, float DS_OUT *val )
{
  int i;
  double d = 0.0,power = 1.0;
  int sign = 1;
  for (i = 0; i < len; i++)
    {
      if (src[i] == ' ') continue;
      if (src[i] == '-') { sign = -1; continue; }
      if (src[i] == '+') { sign = 1; continue; }
      break;
    }
  if (i == (len - 1))
    return -DS_FAULT;
  /* read the integer part */
  for (; i < len; i++)
    {
      if ((src[i] >= '0')&&(src[i] <= '9'))
        d = d* 10.0 + (src[i] - '0');
      else if (src[i] == '.')
        {
          /* read the float part */
          for (i++; i < len; ++i)
            {
              if ((src[i] >= '0')&&(src[i] <= '9'))
                {
                  d = d* 10.0 + (src[i] - '0');
                  power *= 10;
                }
              else
                break;
            }
          break;
        }
      else
        break;
    }
  *val = sign*d/power;
  return 0;
}

/**
 * Convert the string to boolean.
 * @return status code.
 * @param src           Pointer to source string buffer.
 * @param len           Length of string.
 * @parma bl            Where to store the result.
 */
int ds_atob(const char *src, int len, bool DS_OUT *bl)
{ UNUSED(len);
  int istrue = 0 == ds_strcasecmp( src, "true" );
  int isfalse = 0 == ds_strcasecmp( src, "false" );

  if (istrue || isfalse)
    {
      *bl = istrue;
      return 0;
    }
  return -DS_FAULT;
}

/**
 * Convert the integer to string.
 * a 'null' character will be added to the end of string.
 * @return status code.
 * @param src           The source integer value.
 * @param radix         The radix by which the integer is shown in string.
 * @param buff          Pointer to the string buffer.
 * @parma len           The length of string huffer.
 * @param outlen        Optional, where to store the length of output.
 */
int ds_itoa( int src, int radix, char DS_OUT *buff, int len, int DS_OUT *outlen )
{
  int wlen = 0;
  char *p;
  char *firstdig;
  unsigned digval;
  if (len < 2)
    return -DS_OVERFLOW;
  p = buff;
  /* process the sign */
  if (src < 0)
    {
      *p++ = '-';
      src = (unsigned int)(-(int)src);
    }
  firstdig = p;
  /* do the conversation */
  wlen = 1;
  do
    {
      digval = (unsigned)(src % radix);
      src /= radix;
      if (digval > 9)
         *p++ = (char)(digval - 10 + 'a');
      else *p++ = (char)(digval + '0');
      if (wlen++ > len)
        return -DS_OVERFLOW;
    }
  while (src > 0);

  if (outlen)
    *outlen = wlen;
  if (wlen > len)
    return -DS_OVERFLOW;
  else
    {
      /* reserve all */
      char tmp;
      *p-- = '\0';
      do{ tmp = *p;
          *p = *firstdig;
          *firstdig = tmp;
          --p;
          ++firstdig;
      } while (firstdig < p);
    }
  return 0;
}

/**
 * Convert the float to string.
 * a 'null' character will be added to the end of string.
 * @return status code.
 * @param src           The source integer value.
 * @param radix         The radix by which the integer is shown in string.
 *                      A float number can be ff.ff(16h) or 17.17(10h).
 * @param prec          The precision. Must be counted in power. eg. 1000(10^3)
 * @param buff          Pointer to the string buffer.
 * @parma len           The length of string huffer.
 * @param outlen        Optional, where to store the length of output.
 */
int ds_ftoa( float src, int radix, long precision, char DS_OUT *buff, int len, int DS_OUT *outlen )
{
  int rc;
  int outl = 0, outl1 = 0;
  int int_part = (int)src;
  int float_part = (int) ((src - (float)int_part) * (float)precision);

  rc = ds_itoa( int_part, radix, buff, len, &outl ); /* convert the integer part */
  UPDATE_RC(rc);

  buff += outl;
  buff--; /* to skip '\0' that counted in outl. */
  len -= outl;
  /* insert point */
  *(buff++) = '.';
  outl++;
  len--;
  if (len < 0)
    return -DS_OVERFLOW;

  /* convert the float part */
  if( (rc = ds_itoa(float_part, radix, buff, len, &outl1)) )
    return rc;

  if (outlen)
    *outlen = outl + outl1;
  return 0;
}

/**
 * Convert the boolean to string.
 * a 'null' character will be added to the end of string.
 * @return status code.
 * @param src           The source boolean value.
 * @param buff          Pointer to the string buffer.
 * @parma len           The length of string huffer.
 * @param outlen        Optional, where to store the length of output.
 */
int ds_btoa( bool src, char DS_OUT *buff, int len, int DS_OUT *outlen )
{
  int wlen = src?     sizeof("true") : sizeof("false");
  const char *dispaly = src? "true" :         "false";

  if (wlen > len)
    return -DS_OVERFLOW;
  else
    {
      memcpy( buff, dispaly, wlen );
      if (outlen)
          *outlen = wlen;
      return 0;
    }
}
