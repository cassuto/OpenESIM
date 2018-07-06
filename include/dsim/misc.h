/** @file
 * util misc, including macro definitions for platforms,math,type casting
 * memory alignment and so on.
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

#ifndef DS_MISC_H_
#define DS_MISC_H_

#include "autoconf.h"

////////////////////////////////////////////////////////////////////////////////

/** @def ARCH()
 * wheather or not we're compiling with the target CPU architecture.
 */
#define ARCH(type)  (defined ARCH_##type && ARCH_##type)
/** @def OS()
 * wheather or not we're compiling with the target OS.
 */
#define OS(type)    (defined OS_##type && OS_##type)
/** @def ENDIAN()
 * indicates which endian the target supported
 */
#define ENDIAN(type)  (defined ENDIAN_##type && ENDIAN_##type)
/** @def USES()
 * wheather or not to use someting
 */
#define USES(obj)    (defined USES_##obj && USES_##obj)
/** @def HAVE()
 * indicates whether the host machine supports 'obj'
 */
#define HAVE(obj)   (defined HAVE_##obj && HAVE_##obj)
/** @def ENABLE()
 * indicates whether the 'obj' is enabled
 */
#define ENABLE(obj) (defined ENABLE_##obj && ENABLE_##obj)
/** @def USE()
 * indicates whether the 'obj' is used
 */
#define USE(obj) (defined USE_##obj && USE_##obj)

////////////////////////////////////////////////////////////////////////////////

/** @def COMPILER()
 * indicates whether we're compiling with 'cmp' compiler
 */
#define COMPILER(cmp) (defined COMPILER_##cmp && COMPILER_##cmp)
/** @def COMPILER_SUP()
 * indicates whether the compiler supported 'obj' feature
 */
#define COMPILER_SUP(obj) (defined COMPILER_SUP_##obj && COMPILER_SUP_##obj)


/**
 determine the current compilers
 */
#if defined(__GNUC__)
#define COMPILER_GCC 1
#elif defined(_MSC_VER)
#define COMPILER_MSC 1
#endif


/** @def LIKELY
 * Branch prediction. Likely
 * @returns the value of expression.
 * @param   expr  The expression.
 */

/** @def UNLIKELY
 * Branch prediction. Unlikely
 * @returns the value of expression.
 * @param   expr  The expression.
 */

#if COMPILER(GCC)
# if __GNUC__ >= 3 && !defined(FORTIFY_RUNNING)
#  ifndef LIKELY
#  define LIKELY(expr)       __builtin_expect(!!(expr), 1)
#  endif
#  ifndef UNLIKELY
#  define UNLIKELY(expr)     __builtin_expect(!!(expr), 0)
#  endif
# else
#  ifndef LIKELY
#  define LIKELY(expr)       (expr)
#  endif
#  ifndef UNLIKELY
#  define UNLIKELY(expr)     (expr)
#  endif
# endif
#else
# ifndef LIKELY
# define LIKELY(expr)       (expr)
# endif
# ifndef UNLIKELY
# define UNLIKELY(expr)     (expr)
# endif
#endif


/** @def MAX
 * Finds the maximum value.
 * @returns The higher of the two.
 * @param   op1      Value 1
 * @param   op2      Value 2
 */

/** @def MIN
 * Finds the minimum value.
 * @returns The lower of the two.
 * @param   op1      Value 1
 * @param   op2      Value 2
 */
#ifndef MAX
# define MAX(op1, op2)   ((op1) >= (op2) ? (op1) : (op2))
#endif
#ifndef MIN
# define MIN(op1, op2)   ((op1) <= (op2) ? (op1) : (op2))
#endif


/** @def GET_OFFSETOF
 * special offsetof() variant, returns a signed result.
 *
 * This differs from the usual offsetof() in that it's not relying on builtin
 * compiler stuff and thus can use variables in arrays the structure may
 * contain. This is useful to determine the sizes of structures ending
 * with a variable length field.
 *
 * @returns offset into the structure of the specified member. signed.
 * @param   type    Structure type.
 * @param   member  Member.
 */
#define GET_OFFSETOF(type, member)  ( (int)(uintptr_t)&( ((type *)(void *)0)->member) )


/** @def GET_ELEMENTS
 * Calculates the number of elements in a statically sized array.
 * @returns Element count.
 * @param   aArray      Array in question.
 */
#define GET_ELEMENTS(aArray)         ( sizeof(aArray) / sizeof((aArray)[0]) )

/** @def UNUSED
 * Avoid the 'unused parameter' warning.
 */
#define UNUSED(var) (void)var

/** @def DS_CURRENT_FUNCTION
 * Default compiler macros to figure out the name of current function.
 */
#if COMPILER(GCC)
#define DS_CURRENT_FUNCTION __PRETTY_FUNCTION__
#else
#define DS_CURRENT_FUNCTION __FUNCTION__
#endif
#define DS_CURRENT_FILE __FILE__
#define DS_CURRENT_LINE __LINE__



/** @def DebugBreakPoint
 * Emit a debug breakpoint instruction.
 */
#if COMPILER(GCC)
# if ARCH(AMD64) || ARCH(X86)
#  if !defined(__L4ENV__)
#   define DebugBreakPoint()      __asm__ __volatile__("int $3\n\tnop\n\t")
#  else
#   define DebugBreakPoint()      __asm__ __volatile__("int3; jmp 1f; 1:\n\t")
#  endif
# elif ARCH(SPARC64)
#  define DebugBreakPoint()       __asm__ __volatile__("illtrap 0\n\t")   /** @todo Sparc64: this is just a wild guess. */
# elif ARCH(SPARC)
#  define DebugBreakPoint()       __asm__ __volatile__("unimp 0\n\t")     /** @todo Sparc: this is just a wild guess (same as Sparc64, just different name). */
# endif
#elif COMPILER(MSC)
# define DebugBreakPoint()        __debugbreak()
#else
# error "This compiler/arch is not supported!"
#endif


/** @def NULL
 * Null pointer
 */
#ifndef NULL
# ifdef __cplusplus
# define NULL 0
#else
# define NULL (void*)0
#endif
#endif

/** @def callback_decl
 * Declare an internal assembly function.
 * @param   type    The return type of the function declaration.
 */
#ifdef __cplusplus
# if COMPILER(MSC) || OS(OS2)
#  define callback_decl(type)          extern "C" type __cdecl
# elif COMPILER(GCC) && ARCH(X86)
#  define callback_decl(type)          extern "C" type __attribute__((cdecl,regparm(0)))
# else
#  define callback_decl(type)          extern "C" type
# endif
#else
# if COMPILER(MSC) || OS(OS2)
#  define callback_decl(type)          type __cdecl
# elif COMPILER(GCC) && ARCH(X86)
#  define callback_decl(type)          type __attribute__((cdecl,regparm(0)))
# else
#  define callback_decl(type)          type
# endif
#endif

/** @def asm_decl
 * Declare an internal assembly function.
 * @param   type    The return type of the function declaration.
 */
#ifdef __cplusplus
# if COMPILER(MSC) || OS(OS2)
#  define asm_decl(type)          extern "C" type __cdecl
# elif COMPILER(GCC) && ARCH(X86)
#  define asm_decl(type)          extern "C" type __attribute__((cdecl,regparm(0)))
# else
#  define asm_decl(type)          extern "C" type
# endif
#else
# if COMPILER(MSC) || OS(OS2)
#  define asm_decl(type)          type __cdecl
# elif COMPILER(GCC) && ARCH(X86)
#  define asm_decl(type)          type __attribute__((cdecl,regparm(0)))
# else
#  define asm_decl(type)          type
# endif
#endif

#define DS_OUT

#define GET_CONTAINER_OF(ptr, type, member) ({            \
      const typeof( ((type *)0)->member ) *__mptr = (ptr);    \
      (type *)( (char *)__mptr - GET_OFFSETOF(type, member) );})

/** @def DebugBreakPoint
 * Emit a debug breakpoint instruction.
 */
#if COMPILER(GCC)
# if ARCH(AMD64) || ARCH(X86)
#  if !defined(__L4ENV__)
#   define DebugBreakPoint()      __asm__ __volatile__("int $3\n\tnop\n\t")
#  else
#   define DebugBreakPoint()      __asm__ __volatile__("int3; jmp 1f; 1:\n\t")
#  endif
# elif ARCH(SPARC64)
#  define DebugBreakPoint()       __asm__ __volatile__("illtrap 0\n\t")   /** @todo Sparc64: this is just a wild guess. */
# elif ARCH(SPARC)
#  define DebugBreakPoint()       __asm__ __volatile__("unimp 0\n\t")     /** @todo Sparc: this is just a wild guess (same as Sparc64, just different name). */
# endif
#elif COMPILER(MSC)
# define DebugBreakPoint()        __debugbreak()
#else
/* This compiler/arch is not supported! */
# define DebugBreakPoint()        ((void)0)
#endif

#endif //!defined(DS_MISC_H_)
