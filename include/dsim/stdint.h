/** @file
 * stdint.h wrapper.
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

#ifndef DSIM_STDINT_H_
#define DSIM_STDINT_H_

#include <dsim/misc.h>
#include <dsim/cdecl.h>

C_DECLS

/*
 * Use the stdint.h on systems that have one.
 */
#if !(OS(LINUX) && defined(__KERNEL__))  \
  && !(OS(FREEBSD) && defined(_KERNEL)) \
  && !COMPILER(MSC) \
  && HAVE(STDINT_H) \
  && !defined(DOXYGEN_RUNNING)

# ifndef __STDC_CONSTANT_MACROS
#  define __STDC_CONSTANT_MACROS
# endif
# ifndef __STDC_LIMIT_MACROS
#  define __STDC_LIMIT_MACROS
# endif
# include <stdint.h>

# if OS(DARWIN) && defined(KERNEL) && ARCH(AMD64)
 /*
  * Kludge to fix the incorrect 32-bit constant macros in
  * Kernel.framework/Headers/stdin.h. uint32_t and int32_t are
  * int not long as these macros use, which is significant when
  * targeting AMD64. (10a222)
  */
#  undef  INT32_C
#  define INT32_C(Value)    (Value)
#  undef  UINT32_C
#  define UINT32_C(Value)   (Value ## U)
# endif /* 64-bit darwin kludge. */

#elif OS(FREEBSD) && defined(_KERNEL)

# ifndef __STDC_CONSTANT_MACROS
#  define __STDC_CONSTANT_MACROS
# endif
# ifndef __STDC_LIMIT_MACROS
#  define __STDC_LIMIT_MACROS
# endif
# include <sys/stdint.h>

#else /* No system stdint.h */

/*
 * Define the types we use.
 * The linux kernel defines all these in linux/types.h, so skip it.
 */
# if !(OS(LINUX) && defined(__KERNEL__)) \
  || defined(UTIL_NO_STDINT_H) \
  || defined(DOXGEN_RUNNING)

    /* Simplify the [u]int64_t type detection mess. */
# undef UTIL_STDINT_USE_STRUCT_FOR_64_BIT_TYPES

    /* x-bit types */
#  if ARCH(AMD64) || ARCH(X86) || ARCH(SPARC) || ARCH(SPARC64)
#   if !defined(_INT8_T_DECLARED)   && !defined(_INT8_T)
typedef signed char         int8_t;
#   endif
#   if !defined(_UINT8_T_DECLARED)  && !defined(_UINT8_T)
typedef unsigned char       uint8_t;
#   endif
#   if !defined(_INT16_T_DECLARED)  && !defined(_INT16_T)
typedef signed short        int16_t;
#   endif
#   if !defined(_UINT16_T_DECLARED) && !defined(_UINT16_T)
typedef unsigned short      uint16_t;
#   endif
#   if !defined(_INT32_T_DECLARED)  && !defined(_INT32_T)
typedef signed int          int32_t;
#   endif
#   if !defined(_UINT32_T_DECLARED) && !defined(_UINT32_T)
typedef unsigned int        uint32_t;
#   endif
#   if COMPILER(MSC)
#    if !defined(_INT64_T_DECLARED)  && !defined(_INT64_T)
typedef signed _int64       int64_t;
#    endif
#    if !defined(_UINT64_T_DECLARED) && !defined(_UINT64_T)
typedef unsigned _int64     uint64_t;
#    endif
#   elif defined(UTIL_STDINT_USE_STRUCT_FOR_64_BIT_TYPES)
#    if !defined(_INT64_T_DECLARED)  && !defined(_INT64_T)
typedef struct { uint32_t lo; int32_t hi; }     int64_t;
#    endif
#    if !defined(_UINT64_T_DECLARED) && !defined(_UINT64_T)
typedef struct { uint32_t lo; uint32_t hi; }    uint64_t;
#    endif
#   else /* Use long long for 64-bit types */
#    if !defined(_INT64_T_DECLARED)  && !defined(_INT64_T)
typedef signed long long    int64_t;
#    endif
#    if !defined(_UINT64_T_DECLARED) && !defined(_UINT64_T)
typedef unsigned long long  uint64_t;
#    endif
#   endif

    /* max integer types */
#   if !defined(_INTMAX_T_DECLARED)  && !defined(_INTMAX_T)
typedef int64_t             intmax_t;
#   endif
#   if !defined(_UINTMAX_T_DECLARED) && !defined(_UINTMAX_T)
typedef uint64_t            uintmax_t;
#   endif

#  else
#   error "PORTME: Add architecture. Don't forget to check the [U]INTx_C() and [U]INTMAX_MIN/MAX macros."
#  endif

# endif /* !linux kernel or stuff */

    /* pointer <-> integer types */
# if !COMPILER(MSC) || defined(DOXYGEN_RUNNING)
#  if ARCH_BITS == 32 \
   || OS(LINUX) \
   || OS(FREEBSD)
#   if !defined(_INTPTR_T_DECLARED)  && !defined(_INTPTR_T)
typedef signed long         intptr_t;
#   endif
#   if !defined(_UINTPTR_T_DECLARED) && !defined(_UINTPTR_T)
typedef unsigned long       uintptr_t;
#   endif
#  else
#   if !defined(_INTPTR_T_DECLARED)  && !defined(_INTPTR_T)
typedef int64_t             intptr_t;
#   endif
#   if !defined(_UINTPTR_T_DECLARED) && !defined(_UINTPTR_T)
typedef uint64_t            uintptr_t;
#   endif
#  endif
# endif /* !COMPILER(MSC) */

#endif /* no system stdint.h */


/*
 * Make sure the [U]INTx_C(c) macros are present.
 * For In C++ source the system stdint.h may have skipped these if it was
 * included before we managed to define __STDC_CONSTANT_MACROS. (Kludge alert!)
 */
#if !defined(INT8_C) \
 || !defined(INT16_C) \
 || !defined(INT32_C) \
 || !defined(INT64_C) \
 || !defined(INTMAX_C) \
 || !defined(UINT8_C) \
 || !defined(UINT16_C) \
 || !defined(UINT32_C) \
 || !defined(UINT64_C) \
 || !defined(UINTMAX_C)
# define INT8_C(Value)      (Value)
# define INT16_C(Value)     (Value)
# define INT32_C(Value)     (Value)
# define INT64_C(Value)     (Value ## LL)
# define UINT8_C(Value)     (Value)
# define UINT16_C(Value)    (Value)
# define UINT32_C(Value)    (Value ## U)
# define UINT64_C(Value)    (Value ## ULL)
# define INTMAX_C(Value)    INT64_C(Value)
# define UINTMAX_C(Value)   UINT64_C(Value)
#endif


/*
 * Make sure the INTx_MIN and [U]INTx_MAX macros are present.
 * For In C++ source the system stdint.h may have skipped these if it was
 * included before we managed to define __STDC_LIMIT_MACROS. (Kludge alert!)
 */
#if !defined(INT8_MIN) \
 || !defined(INT16_MIN) \
 || !defined(INT32_MIN) \
 || !defined(INT64_MIN) \
 || !defined(INT8_MAX) \
 || !defined(INT16_MAX) \
 || !defined(INT32_MAX) \
 || !defined(INT64_MAX) \
 || !defined(UINT8_MAX) \
 || !defined(UINT16_MAX) \
 || !defined(UINT32_MAX) \
 || !defined(UINT64_MAX)
# define INT8_MIN           (INT8_C(-0x7f)                - 1)
# define INT16_MIN          (INT16_C(-0x7fff)             - 1)
# define INT32_MIN          (INT32_C(-0x7fffffff)         - 1)
# define INT64_MIN          (INT64_C(-0x7fffffffffffffff) - 1)
# define INT8_MAX           INT8_C(0x7f)
# define INT16_MAX          INT16_C(0x7fff)
# define INT32_MAX          INT32_C(0x7fffffff)
# define INT64_MAX          INT64_C(0x7fffffffffffffff)
# define UINT8_MAX          UINT8_C(0xff)
# define UINT16_MAX         UINT16_C(0xffff)
# define UINT32_MAX         UINT32_C(0xffffffff)
# define UINT64_MAX         UINT64_C(0xffffffffffffffff)

# define INTMAX_MIN         INT64_MIN
# define INTMAX_MAX         INT64_MAX
# define UINTMAX_MAX        UINT64_MAX
#endif

END_C_DECLS

#endif //!defined(DSIM_STDINT_H_)
