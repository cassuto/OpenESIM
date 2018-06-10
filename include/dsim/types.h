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

#ifndef DS_TYPES_H_
#define DS_TYPES_H_

#include <dsim/misc.h>
#include <dsim/stdint.h>

/*
 Run-time library defined standard C types.
 */
#if OS(LINUX) && defined(__KERNEL__)
#else
# include <stddef.h>
# include <sys/types.h>
# if HAVE(INTTYPES)
#   define __STDC_FORMAT_MACROS
#   include <inttypes.h>
# else
#   define PRId64 "%lld"
# endif
#endif

/** @def NULL
 * NULL/nil pointer.
 */
#ifndef NULL
# ifdef __cplusplus
#  define NULL 0
# else
#  define NULL ((void*)0)
# endif
#endif

/**
 * Bool adaptation for C++(exported) -> C(imported)
 */
#ifndef __cplusplus
#if defined(__GNUC__)
#  if OS(LINUX) && __GNUC__ < 3
typedef uint8_t bool;
#  elif OS(FREEBSD)
#   ifndef __bool_true_false_are_defined
typedef _Bool bool;
#   endif
#  else
#   if OS(DARWIN) && defined(_STDBOOL_H)
#    undef bool
#   endif
typedef _Bool bool;
#  endif

#else
typedef unsigned char bool;
#endif

#ifndef true
# define true  (1)
#endif
#ifndef false
# define false (0)
#endif
#endif

/**
 file offset internal
 */
typedef uint64_t file_off;

/**
 * 64-bit unsigned integer union.
 */
typedef union UINT64U {
    /** Natural view. */
    uint64_t    u;
    /** Hi/Low view. */
    struct {
#if ENDIAN(BIG)
        uint32_t    hi;
        uint32_t    lo;
#else
        uint32_t    lo;
        uint32_t    hi;
#endif
    } s;
    /** Double-Word view. */
    struct {
#if ENDIAN(BIG)
        uint32_t    dw1;
        uint32_t    dw0;
#else
        uint32_t    dw0;
        uint32_t    dw1;
#endif
    } dwords;
    /** Word view. */
    struct {
#if ENDIAN(BIG)
        uint16_t    w3;
        uint16_t    w2;
        uint16_t    w1;
        uint16_t    w0;
#else
        uint16_t    w0;
        uint16_t    w1;
        uint16_t    w2;
        uint16_t    w3;
#endif
    } words;

    /** 64-bit view. */
    uint64_t    au64[1];
    /** 32-bit view. */
    uint32_t    au32[2];
    /** 16-bit view. */
    uint16_t    au16[4];
    /** 8-bit view. */
    uint8_t     au8[8];
} UINT64U;


/**@def MAKE_UINT32
 * Make a uint32 number from two uint16 numbers.
 */
#define MAKE_UINT32(a, b) ((uint32_t)(((uint16_t)(a)) | ((uint32_t)((uint16_t)(b))) << 16))

#endif //!defined(DS_TYPES_H_)
