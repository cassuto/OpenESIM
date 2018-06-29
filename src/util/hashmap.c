/** @file
 * unordered hash map implement
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

#define TRACE_UNIT "hashmap"

#include <dsim/memory.h>
#include <dsim/error.h>
#include <dsim/string.h>
#include <dsim/logtrace.h>
#include <dsim/bound.h>
#include "prime.h"

#include <dsim/hashmap.h>

static unsigned long hash_string( hashmap_key_t key );
static unsigned long hash_number( hashmap_key_t key );
static bool comp_string( hashmap_key_t src, hashmap_key_t dst );
static bool comp_number( hashmap_key_t src, hashmap_key_t dst );

static pfnKeyHash hash_funcs[] =
{
    NULL,        /* HASHMAP_KEY_INVALID */
    hash_string, /* HASHMAP_KEY_CHR */
    hash_number, /* HASHMAP_KEY_INT */
    NULL
};
static pfnKeyComp comp_funcs[] =
{
    NULL,        /* HASHMAP_KEY_INVALID */
    comp_string, /* HASHMAP_KEY_CHR */
    comp_number, /* HASHMAP_KEY_INT */
    NULL
};


static unsigned long
hash_string( hashmap_key_t key )
{
  const char *str = (const char *)key;
  unsigned int hash = 0;
  unsigned int x    = 0;
  unsigned int i    = 0;
  unsigned int len = strlen( str );

  for(i = 0; i < len; str++, i++)
    {
      hash = (hash << 4) + (*str);
      if((x = hash & 0xF0000000L) != 0)
        {
          hash ^= (x >> 24);
        }
      hash &= ~x;
    }
  return hash;
}

static unsigned long
hash_number( hashmap_key_t key )
{
  return (unsigned long)(key);
}

static bool
comp_string( hashmap_key_t src, hashmap_key_t dst )
{
  return (0 == strcmp( (const char*)src, (const char *)dst ));
}

static bool
comp_number( hashmap_key_t src, hashmap_key_t dst )
{
  return ((intptr_t)src == (intptr_t)dst);
}

/* C++ STL Prime_rehash_policy
 * return < 0 if no need
 * return > 0 the new size
 */
static inline int
need_rehash( hashmap_t *hashmap, int n_bkt )
{
  int n_elt = hashmap->element_count;

  if (n_elt + 1 > hashmap->next_resize)
    {
      float min_bkts = ((1.0f + (float)n_elt) / hashmap->max_load_factor);

      if (min_bkts > n_bkt)
        {
          min_bkts = MAX(min_bkts, hashmap->growth_factor * n_bkt);
          const unsigned long* p = lower_bound(prime_list, prime_list + _S_n_primes, (unsigned long)min_bkts);
          hashmap->next_resize = (int)(*p * hashmap->max_load_factor);
          return *p;
        }
      else
        {
          hashmap->next_resize = (int)(n_bkt * hashmap->max_load_factor);
          return -1;
        }
    }
  else
    return -1;
}

/* init_size = initial bucket count (could be zero) */
int
hashmap_init( hashmap_t *hashmap, hashmap_type_t keytype, size_t init_size )
{
  trace_assert( keytype > HASHMAP_KEY_INVALID && keytype<= HASHMAP_KEY_INTPTR );

  if( init_size )
    {
      /* Allocate buckets */
      hashmap->nodes = (hashmap_node_t**)ds_heap_alloc( init_size * sizeof(hashmap_node_t*) );
      if ( ! hashmap->nodes )
        return -DS_NO_MEMORY;
      memset( hashmap->nodes, 0, init_size * sizeof(hashmap_node_t*) );
    }
  else
    hashmap->nodes = NULL;
  hashmap->keytype = keytype;
  hashmap->size = init_size;
  hashmap->element_count = 0;
  hashmap->hash_func = hash_funcs[keytype];
  hashmap->comp_func = comp_funcs[keytype];
  hashmap->next_resize = init_size;
  hashmap->max_load_factor = 1.0f;
  hashmap->growth_factor = 2.0f;
  hashmap->collected_root = NULL;
  hashmap->collected_tail = NULL;

  return 0;
}

static void
rehash( hashmap_t *hashmap, int newsize )
{
  hashmap_node_t** ns = (hashmap_node_t**)ds_heap_alloc( newsize * sizeof(hashmap_node_t*) );
  if( ns )
    {
      int id;
      hashmap_node_t *node, *cur;

      memset( ns, 0, newsize * sizeof(hashmap_node_t*) );

      /* recalculate the bucket id of each element */
      for (int i = 0; i < hashmap->size; i++)
        {
          node = hashmap->nodes[i];
          while ( node )
            {
              cur = node; node = node->next;

              id = hashmap->hash_func( cur->key ) % newsize;

              cur->next = ns[id];
              ns[id] = cur;
            }
        }

      ds_heap_free( hashmap->nodes );
      hashmap->nodes = ns;
      hashmap->size = newsize;
    }
}

void
hashmap_insert( hashmap_t *hashmap, hashmap_key_t key, hashmap_node_t *node )
{
  int id;
  int rc = need_rehash( hashmap, hashmap->size );
  if ( rc > 0 )
    {
      rehash( hashmap, rc );
    }

  id = hashmap->hash_func( key ) % hashmap->size;

  node->key = key;

  node->next = hashmap->nodes[id];
  hashmap->nodes[id] = node;

  hashmap->element_count++;
}

hashmap_node_t *
hashmap_at( hashmap_t *hashmap, hashmap_key_t key )
{
  int hash = hashmap->hash_func( key );
  pfnKeyComp comp_func = hashmap->comp_func;
  hashmap_node_t *node;
  if( !hashmap->element_count ) return NULL;

  node = hashmap->nodes[hash % hashmap->size];

  while ( node )
    {
      if ( comp_func( key, node->key ) )
        return node;
      node = node->next;
    }

  return NULL;
}

/* if free_elem==NULL then collect the memory for the next allocation */
int
hashmap_remove( hashmap_t *hashmap, hashmap_key_t key, pfn_free_elem free_elem )
{
  int hash = hashmap->hash_func( key );
  pfnKeyComp comp_func = hashmap->comp_func;
  hashmap_node_t *node, *lastnode = NULL;

  node = hashmap->nodes[hash % hashmap->size];

  while ( node )
    {
      if ( comp_func( key, node->key ) )
        {
          if ( LIKELY( lastnode ) )
            lastnode->next = node->next;
          else
            hashmap->nodes[hash % hashmap->size] = node->next;
          if ( free_elem ) free_elem( node );
          else hashmap_collect( hashmap, node );

          hashmap->element_count --;
          trace_assert(hashmap->element_count >= 0);
          return 0;
        }

      lastnode = node;
      node = node->next;
    }

  return -DS_FAULT;
}

/* if free_elem==NULL then collect the memory for the next allocation */
void
hashmap_clear( hashmap_t *hashmap, pfn_free_elem free_elem )
{
  for (int i = 0; i < hashmap->size; i++)
    {
      if ( free_elem )
        {
          hashmap_node_t *node = hashmap->nodes[i], *cur = NULL;
          while ( node )
            {
              cur = node;
              node = cur->next;
              free_elem( cur );
            }
        }
      else if( hashmap->nodes[i] )
        {
          /* collect the whole bucket elements without free */
          if ( LIKELY(hashmap->collected_tail) )
            hashmap->collected_tail->next = hashmap->nodes[i];
          else
            hashmap->collected_root = hashmap->nodes[i];
          hashmap->collected_tail = hashmap->nodes[i];
        }
      hashmap->nodes[i] = NULL;
    }
  hashmap->element_count = 0;
}

void
hashmap_release( hashmap_t *hashmap, pfn_free_elem free_elem )
{
  /* free collected elements */
  hashmap_node_t *node = hashmap->collected_root, *cur;
  while( node )
    {
      cur = node;
      node = node->next;

      free_elem( cur );
    }
  hashmap->collected_root = hashmap->collected_tail = NULL;

  hashmap_clear( hashmap, free_elem );
  ds_heap_free( hashmap->nodes );
}
