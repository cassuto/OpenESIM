/** @file
 * Memory management
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

#define TRACE_UNIT "heap"

#include <dsim/logtrace.h>
#include <dsim/misc.h>
#include <dsim/memory.h>
#include <dsim/magic.h>

#include <stdlib.h>
#include <string.h>

#define ENABLE_HEAP_TRACK 1

#if ENABLE(HEAP_TRACK)

typedef struct heap_block_s
{
  ds_magic_t magic;
  size_t size;
} heap_block_t;

/* estimate statistics of heap blocks */
size_t current_heap_size = 0;
size_t peak_heap_size = 0;

#endif


/* Allocate a memory block on the heap */
void *
ds_heap_alloc (size_t size)
{
#if ENABLE(HEAP_TRACK)
  void *ptr = malloc (size + sizeof(heap_block_t)), *block;
#else
  void *ptr = malloc (size);
#endif
  trace_assert( size ); /* zero allocation is not allowed */
  if( ptr )
    {
#if ENABLE(HEAP_TRACK)
      heap_block_t *hp = (heap_block_t *)ptr;
      hp->magic = MAGIC_MEM_BLOCK; /* set magic number */
      hp->size = size;
      current_heap_size += size;

      /* Update the peak value of heap size */
      if ( current_heap_size > peak_heap_size)
        peak_heap_size = current_heap_size;

      block = (char*)ptr + sizeof(heap_block_t);
      memset( block, 0xbd, size ); /* set initial value */
      return block;
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
  if( mem )
    {
      if( !newsize ) /* free the original memory */
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

/* free a heap block with NULL pointer checking. */
void
ds_heap_free (void *mem)
{
  void *ptr = mem;
  if( mem )
    {
#if ENABLE(HEAP_TRACK)
      ptr = mem - sizeof(heap_block_t);
      heap_block_t *hp = (heap_block_t *)(ptr);

      /* validate the magic number of memory block */
      if( hp->magic != MAGIC_MEM_BLOCK )
        {
          trace_panic(("ds_heap_free() invalid memory block %p that going to free\n", mem));
        }
      current_heap_size -= hp->size;
      hp->magic = 0xbadbeeef;

#if 0
      trace_debug(("free %u bytes\n", hp->size));
      if ( hp->size > 8192 )
        {
          __asm__ __volatile__("int $3\n\tnop\n\t");
        }
      if( (uintptr_t)mem == 0x003E01A0 )
          __asm__ __volatile__("int $3\n\tnop\n\t");
      trace_info(("%p = mem\n", mem));
#endif
      memset( mem, 0xbd, hp->size ); /* destroy the original data */
#endif // ENABLE(HEAP_TRACK)

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
