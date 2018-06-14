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

#ifndef DSIM_MEMORY_H_
#define DSIM_MEMORY_H_

#include <dsim/types.h>

void *ds_heap_alloc( size_t size );
void *ds_heap_realloc( void *mem, size_t newsize );
void ds_heap_free( void *mem );

void ds_heap_print_stat( void );

#endif //!defined(DSIM_MEMORY_H_)
