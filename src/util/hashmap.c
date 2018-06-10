/** @file
 * Hash map implement
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

#define TRACE_UNIT "hashmap"

#include <dsim/memory.h>
#include <dsim/error.h>
#include <dsim/string.h>
#include <dsim/logtrace.h>

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

////////////////////////////////////////////////////////////////////////////////

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
  return ((uintptr_t)src == (uintptr_t)dst);
}

hashmap_t *
hashmap_create( hashmap_type_t keytype, size_t size )
{
  if ( !(keytype > HASHMAP_KEY_INVALID && keytype<= HASHMAP_KEY_INT) )
    return NULL;

  hashmap_t * hashmap = (hashmap_t *)ds_heap_alloc( sizeof(*hashmap) );
  if ( !hashmap ) return NULL;

  hashmap->nodes = (hashmap_node_t**)ds_heap_alloc( size * sizeof(hashmap_node_t*) );
  hashmap->keytype = keytype;
  hashmap->size = size;
  hashmap->hash_func = hash_funcs[keytype];
  hashmap->comp_func = comp_funcs[keytype];

  memset( hashmap->nodes, 0, size * sizeof(hashmap_node_t*) );

  return hashmap;
}

void
hashmap_insert( hashmap_t *hashmap, hashmap_key_t key, void *elem )
{
  int id = hashmap->hash_func( key ) % hashmap->size;
  hashmap_node_t *node = DS_HASHMAP_GET( elem );

  node->key = key;

  node->prev = hashmap->nodes[id];
  hashmap->nodes[id] = node;
}

void *
hashmap_at( hashmap_t *hashmap, hashmap_key_t key )
{
  int hash = hashmap->hash_func( key );
  pfnKeyComp comp_func = hashmap->comp_func;
  hashmap_node_t *node;

  node = hashmap->nodes[hash % hashmap->size];

  while ( node )
    {
      if ( comp_func( key, node->key ) )
        return node;
      node = node->prev;
    }

  return NULL;
}

bool
hashmap_contain_node( hashmap_t *hashmap, hashmap_key_t key, void *node )
{
  const hashmap_node_t *orig = hashmap_at( hashmap, key );
  return( orig == node );
}

bool
hashmap_contain_key( hashmap_t *hashmap, hashmap_key_t key )
{
  return( NULL!=hashmap_at( hashmap, key ) );
}

int
hashmap_remove( hashmap_t *hashmap, hashmap_key_t key, pfnFreeElem free_elem )
{
  int hash = hashmap->hash_func( key );
  pfnKeyComp comp_func = hashmap->comp_func;
  hashmap_node_t *node, *lastnode = NULL;

  node = hashmap->nodes[hash % hashmap->size];

  while ( node )
    {
      if ( comp_func( key, node->key ) )
        {
          lastnode->prev = node->prev;
          if ( free_elem ) free_elem( node );
          return 0;
        }

      lastnode = node;
      node = node->prev;
    }

  return -DS_FAULT;
}

void
hashmap_free( hashmap_t *hashmap, pfnFreeElem free_elem )
{
  int i = 0;
  hashmap_node_t *node, *cur = NULL;

  if ( free_elem )
    {
      for (i = 0; i < hashmap->size; i++)
        {
          node = hashmap->nodes[i];
          while ( node )
            {
              cur = node;
              node = cur->prev;
              free_elem( cur );
            }
        }
    }

  ds_heap_free( hashmap->nodes );
  ds_heap_free( hashmap );
}
