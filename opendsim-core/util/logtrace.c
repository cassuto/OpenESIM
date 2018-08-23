/** @file
 * Log trace
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

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#include <dsim/logtrace.h>

#include <dsim/string.h>
#include <dsim/misc.h>

#define log_printf ds_printf

static const char *log_unit_name = 0;
static const char *log_unit_filename = 0;
static int log_unit_line = 0;
static ds_log_level_t log_level = LOGTRACE_INFO;


static
void dev_out( unsigned char c )
{
  fputc( c, stdout );
}

static
unsigned char dev_in( void )
{
  return (char)fgetc( stdin );
}

void
ds_log_init( void )
{
  ds_set_dev_out( dev_out );
  ds_set_dev_in( dev_in );
}

void
ds_log_set_unit( const char *unitname, const char *filename, int line )
{
  log_unit_name = unitname ? unitname : "all";
  log_unit_filename = filename ? filename : "unknown";
  log_unit_line = line;
}

void
ds_log_set_level( ds_log_level_t level )
{
  log_level = level;
}

void
ds_log_trace( const char *format, ... )
{
  char buff[2048];
  va_list args;
  va_start (args, format);

  vsnprintf(buff, sizeof(buff), format, args);
  va_end(args);

  if( log_level == LOGTRACE_PANIC )
    log_printf( "Panic! in %s: %d\n", log_unit_filename, log_unit_line );

  log_printf( "[%s] %s", log_unit_name, buff );
}

void
ds_panic( int rc )
{
  UNUSED(rc);
  exit( 1 );
}

#define _USE_XFUNC_OUT  1   /* 1: Use output functions */
#define _CR_CRLF        1   /* 1: Convert \n ==> \r\n in the output char */
#define _USE_LONGLONG   0   /* 1: Enable long long integer in type "ll". */
#define _LONGLONG_t     long long   /* Platform dependent long long integer type */

#define _USE_XFUNC_IN   1   /* 1: Use input function */
#define _LINE_ECHO      1   /* 1: Echo back input chars in xgets function */

#define DW_CHAR     sizeof(char)
#define DW_SHORT    sizeof(short)
#define DW_LONG     sizeof(long)

static char *outptr;


void (*term_func_out)(unsigned char);   /* Pointer to the output stream */

/** Put a character */
void
ds_putc(char c)
{
  if (_CR_CRLF && c == '\n') ds_putc('\r');     /* CR -> CRLF */

  if (outptr)         /* Destination is memory */
    {
      *outptr++ = (unsigned char)c;
      return;
    }
  if (term_func_out)      /* Destination is device */
    {
      term_func_out((unsigned char)c);
    }
}

/** Put a null-terminated string */
void
ds_puts (                    /* Put a string to the default device */
  const char* str             /* Pointer to the string */
)
{
  while (*str)
    {
      ds_putc(*str++);
    }
}


/**
 Formatted string output

 @param fmt Pointer to the format string.
 @param arp Pointer to arguments.

 Example codes:
  ds_printf("%d", 1234);            "1234"
  ds_printf("%6d,%3d%%", -200, 5);  "  -200,  5%"
  ds_printf("%-6u", 100);           "100   "
  ds_printf("%ld", 12345678);       "12345678"
  ds_printf("%llu", 0x100000000);   "4294967296"    <_USE_LONGLONG>
  ds_printf("%04x", 0xA3);          "00a3"
  ds_printf("%08lX", 0x123ABC);     "00123ABC"
  ds_printf("%016b", 0x550F);       "0101010100001111"
  ds_printf("%s", "String");        "String"
  ds_printf("%-5s", "abc");         "abc  "
  ds_printf("%5s", "abc");          "  abc"
  ds_printf("%c", 'a');             "a"
  ds_printf("%f", 10.0);            <ds_printf lacks floating point support. Use regular printf.>
 */

static
void
ds_xvprintf( const char* fmt, va_list arp )
{
  unsigned int r, i, j, w, f;
  char s[24], c, d, *p;
#if _USE_LONGLONG
  _LONGLONG_t v;
  unsigned _LONGLONG_t vs;
#else
  long v;
  unsigned long vs;
#endif

  for (;;)
    {
      c = *fmt++;                 /* Get a format character */
      if (!c) break;              /* End of format? */
      if (c != '%')               /* Pass it through if not a % sequense */
        {
          ds_putc(c);
          continue;
        }
      f = 0;                      /* Clear flags */
      c = *fmt++;                 /* Get first char of the sequense */
      if (c == '0')               /* Flag: left '0' padded */
        {
          f = 1; c = *fmt++;
        }
      else
        {
          if (c == '-')           /* Flag: left justified */
            {
              f = 2; c = *fmt++;
            }
        }
      for (w = 0; c >= '0' && c <= '9'; c = *fmt++)  /* Minimum width */
        {
          w = w * 10 + c - '0';
        }
      if (c == 'l' || c == 'L')   /* Prefix: Size is long */
        {
          f |= 4; c = *fmt++;
#if _USE_LONGLONG
          if (c == 'l' || c == 'L')   /* Prefix: Size is long long */
            {
              f |= 8; c = *fmt++;
            }
#endif
        }
      if (!c) break;              /* End of format? */
      d = c;
      if (d >= 'a') d -= 0x20;
      switch (d)                  /* Type is... */
      {
      case 'S' :                  /* String */
        {
          p = va_arg(arp, char*);
          for (j = 0; p[j]; j++) ;
          while (!(f & 2) && j++ < w) ds_putc(' ');
          ds_puts(p);
          while (j++ < w) ds_putc(' ');
          continue;
        }
      case 'C' :                  /* Character */
          ds_putc((char)va_arg(arp, int)); continue;
      case 'B' :                  /* Binary */
          r = 2; break;
      case 'O' :                  /* Octal */
          r = 8; break;
      case 'D' :                  /* Signed decimal */
      case 'U' :                  /* Unsigned decimal */
          r = 10; break;
      case 'X' :                  /* Hexdecimal */
          r = 16; break;
      default:                    /* Unknown type (passthrough) */
          ds_putc(c); continue;
      }

      /* Get an argument and put it in numeral */
#if _USE_LONGLONG
      if (f & 8)      /* long long argument? */
        {
          v = va_arg(arp, _LONGLONG_t);
        }
      else
        {
          if (f & 4)      /* long argument? */
            {
              v = (d == 'D') ? (long)va_arg(arp, long) : (long)va_arg(arp, unsigned long);
            }
          else          /* int/short/char argument */
            {
              v = (d == 'D') ? (long)va_arg(arp, int) : (long)va_arg(arp, unsigned int);
            }
        }
#else
      if (f & 4)      /* long argument? */
        {
          v = va_arg(arp, long);
        }
      else          /* int/short/char argument */
        {
          v = (d == 'D') ? (long)va_arg(arp, int) : (long)va_arg(arp, unsigned int);
        }
#endif
      if (d == 'D' && v < 0)      /* Negative value? */
        {
          v = 0 - v; f |= 16;
        }
      i = 0; vs = v;
      do
        {
          d = (char)(vs % r); vs /= r;
          if (d > 9) d += (c == 'x') ? 0x27 : 0x07;
          s[i++] = d + '0';
        }
      while (vs != 0 && i < sizeof s);

      if (f & 16) s[i++] = '-';
      j = i; d = (f & 1) ? '0' : ' ';
      while (!(f & 2) && j++ < w) ds_putc(d);
      do ds_putc(s[--i]); while (i != 0);
      while (j++ < w) ds_putc(' ');
    }
}

/** Put a formatted string to the default device
 * @param fmt Pointer to the format string.
 * @param ... Optional arguments
 */
void
ds_printf( const char* fmt, ... )
{
  va_list arp;

  va_start(arp, fmt);
  ds_xvprintf(fmt, arp);
  va_end(arp);
}

/** Put a formatted string to the memory
 * @param buff Pointer to the output buffer.
 * @param fmt Pointer to the format string.
 * @param ... Optional arguments
 */
void
ds_xsprintf( char* buff, const char* fmt, ... )
{
  va_list arp;


  outptr = buff;      /* Switch destination for memory */

  va_start(arp, fmt);
  ds_xvprintf(fmt, arp);
  va_end(arp);

  *outptr = 0;        /* Terminate output string with a \0 */
  outptr = 0;         /* Switch destination for device */
}


/** Dump a line of binary dump */
void
put_dump (
  const void* buff,       /* Pointer to the array to be dumped */
  unsigned long addr,     /* Heading address value */
  int len,                /* Number of items to be dumped */
  int width               /* Size of the items (DF_CHAR, DF_SHORT, DF_LONG) */
)
{
  int i;
  const unsigned char *bp;
  const unsigned short *sp;
  const unsigned long *lp;

  ds_printf("%08lX ", addr);        /* address */

  switch (width)
  {
  case DW_CHAR:
    bp = buff;
    for (i = 0; i < len; i++)       /* Hexdecimal dump */
      ds_printf(" %02X", bp[i]);
    ds_putc(' ');
    for (i = 0; i < len; i++)       /* ASCII dump */
      ds_putc((unsigned char)((bp[i] >= ' ' && bp[i] <= '~') ? bp[i] : '.'));
    break;
  case DW_SHORT:
    sp = buff;
    do                              /* Hexdecimal dump */
      ds_printf(" %04X", *sp++);
    while (--len);
    break;
  case DW_LONG:
    lp = buff;
    do                              /* Hexdecimal dump */
        ds_printf(" %08LX", *lp++);
    while (--len);
    break;
  }

  ds_putc('\n');
}


unsigned char (*term_func_in)(void);    /* Pointer to the input stream */

/** Get a line from the input */
int
ds_gets (     /* 0:End of stream, 1:A line arrived */
  char* buff, /* Pointer to the buffer */
  int len     /* Buffer length */
)
{
  int c, i;

  if (!term_func_in) return 0;        /* No input function specified */

  i = 0;
  for (;;)
    {
      c = term_func_in();             /* Get a char from the incoming stream */
      if (!c) return 0;           /* End of stream? */
      if (c == '\r') break;       /* End of line? */
      if (c == '\b' && i)         /* Back space? */
        {
          i--;
          if (_LINE_ECHO) ds_putc((unsigned char)c);
          continue;
        }
      if (c >= ' ' && i < len - 1)  /* Visible chars */
        {
          buff[i++] = c;
          if (_LINE_ECHO) ds_putc((unsigned char)c);
        }
    }
  buff[i] = 0;    /* Terminate with a \0 */
  if (_LINE_ECHO) ds_putc('\n');
  return 1;
}
