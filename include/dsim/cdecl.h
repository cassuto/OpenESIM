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

#ifndef CDECL_H_
#define CDECL_H_

/** @def BEGIN_C_DECLS
 * Used to start a block of function declarations which are shared
 * between C and C++ program.
 */
#if defined(__cplusplus)
# define C_DECLS extern "C" {
#else
# define C_DECLS
#endif

/** @def END_C_DECLS
 * Used to end a block of function declarations which are shared
 * between C and C++ program.
 */
#if defined(__cplusplus)
# define END_C_DECLS   }
#else
# define END_C_DECLS
#endif

#endif //!defined(CDECL_H_)
