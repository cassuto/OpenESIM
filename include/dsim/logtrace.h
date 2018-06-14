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

#ifndef DSIM_LOGTRACE_H_
#define DSIM_LOGTRACE_H_

#ifndef TRACE_UNIT
# define TRACE_UNIT 0
#endif

typedef enum ds_log_level_e
{
  LOG_PANIC = 0,
  LOG_ERROR,
  LOG_WARNING,
  LOG_INFO,
  LOG_DEBUG
} ds_log_level_t;

#define ds_log_lock()
#define ds_log_unlock()

void ds_log_init( void );
void ds_log_set_unit( const char *unitname, const char *filename, int line );
void ds_log_set_level( ds_log_level_t level );
void ds_log_trace( const char *format, ... );

void ds_panic( int rc );

#define trace_panic(msg) \
  do { \
    ds_log_lock(); \
    ds_log_set_unit (TRACE_UNIT, __FILE__, __LINE__); \
    ds_log_set_level (LOG_PANIC); \
    ds_log_trace msg ; \
    ds_log_unlock(); \
  } while(0)

#define trace_error(msg) \
  do { \
    ds_log_lock(); \
    ds_log_set_unit (TRACE_UNIT, __FILE__, __LINE__); \
    ds_log_set_level (LOG_ERROR); \
    ds_log_trace msg ; \
    ds_log_unlock(); \
  } while(0)

#define trace_warning(msg) \
  do { \
    ds_log_lock(); \
    ds_log_set_unit (TRACE_UNIT, __FILE__, __LINE__); \
    ds_log_set_level (LOG_WARNING); \
    ds_log_trace msg ; \
    ds_log_unlock(); \
  } while(0)

#define trace_info(msg) \
  do { \
    ds_log_lock(); \
    ds_log_set_unit (TRACE_UNIT, __FILE__, __LINE__); \
    ds_log_set_level (LOG_INFO); \
    ds_log_trace msg ; \
    ds_log_unlock(); \
  } while(0)

#if DEBUG_TARCE
# define trace_debug(msg) \
    do { \
      ds_log_lock(); \
      ds_log_set_unit (TRACE_UNIT, __FILE__, __LINE__); \
      ds_log_set_level (LOG_DEBUG); \
      ds_log_trace msg ; \
      ds_log_unlock(); \
    } while(0)
#else
# define trace_debug(msg)
#endif

#define TRACE_ASSERT(expr) \
  do { \
    if (!(expr)) \
      { \
        trace_panic(("assert failure. expt = %s\n", #expr)); \
        ds_panic(1); \
      } \
  } while(0)

static inline void
trace_assert( int expr )
{
  if (!(expr)) \
    { \
      trace_panic(("assert failure.\n")); \
      ds_panic(1); \
    } \
}

void ds_putc (char c);
void ds_puts (const char* str);
void ds_printf (const char* fmt, ...);
void ds_xsprintf (char* buff, const char* fmt, ...);
void put_dump (const void* buff, unsigned long addr, int len, int width);

int ds_gets (char* buff, int len);

extern void (*term_func_out)(unsigned char);
extern unsigned char (*term_func_in)(void);

#define ds_set_dev_out(func) term_func_out = (void(*)(unsigned char))(func)
#define ds_set_dev_in(func) term_func_in = (unsigned char(*)(void))(func)

#endif //!defined(DSIM_LOGTRACE_H_)
