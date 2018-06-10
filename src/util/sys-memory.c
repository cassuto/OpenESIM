/** @file
 * Memory management
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

#define TRACE_UNIT "heap"

#include <dsim/logtrace.h>
#include <dsim/misc.h>
#include <dsim/memory.h>

#include <stdlib.h>

#define ENABLE_HEAP_TRACK 1

#if ENABLE(HEAP_TRACK)

typedef struct heap_block_s
{
  size_t size;
} heap_block_t;

/* estimate statistics of heap blocks */
size_t current_heap_size = 0;
size_t peak_heap_size = 0;

#endif

////////////////////////////////////////////////////////////////////////////////

/* Allocate a memory block on the heap */
void *
ds_heap_alloc (size_t size)
{
#if ENABLE(HEAP_TRACK)
  void *ptr = malloc (size + sizeof(heap_block_t));
#else
  void *ptr = malloc (size);
#endif
  if (ptr)
    {
#if ENABLE(HEAP_TRACK)
      heap_block_t *hp = (heap_block_t *)ptr;
      hp->size = size;
      current_heap_size += size;

      /* Update the peak value of heap size */
      if ( current_heap_size > peak_heap_size)
        peak_heap_size = current_heap_size;

      return (char*)ptr + sizeof(heap_block_t);
#else
      return ptr;
#endif
    }
  return NULL;
}

void *
ds_heap_realloc( void *mem, size_t newsize )
{
  void *ptr = mem, *newptr;
#if ENABLE(HEAP_TRACK)
  ptr = mem - sizeof(heap_block_t);
  newsize += sizeof(heap_block_t);
#endif
  if (mem)
    {
      if (!newsize) /* free the original memory */
        {
          ds_heap_free( ptr );
          return NULL;
        }
      newptr = realloc( ptr, newsize );
#if ENABLE(HEAP_TRACK)
      newptr += sizeof(heap_block_t);
#endif
    }
  else
    newptr = ds_heap_alloc ( newsize );
  return newptr;
}

/* free a heap block with pointer checking. */
void
ds_heap_free (void *mem)
{
  void *ptr = mem;
  if (mem)
    {
#if ENABLE(HEAP_TRACK)
      ptr = mem - sizeof(heap_block_t);
      heap_block_t *hp = (heap_block_t *)(ptr);
      current_heap_size -= hp->size;

#if 0
      trace_debug(("free %u bytes\n", hp->size));
      if ( hp->size > 8192 )
        {
          __asm__ __volatile__("int $3\n\tnop\n\t");
        }
#endif
#endif
      free (ptr);
    }
}

void
ds_heap_print_stat( void )
{
#if ENABLE(HEAP_TRACK)
  trace_info(("current size = %u B\n", current_heap_size));
  trace_info(("peak size = %u B\n", peak_heap_size));
#endif
}
