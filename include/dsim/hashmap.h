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

#ifndef DSIM_HASHMAP_H_
#define DSIM_HASHMAP_H_

#include <dsim/types.h>
#include <dsim/list.h>
#include <dsim/memory.h>
#include <dsim/cdecl.h>

C_DECLS

typedef uintptr_t hashmap_key_t;

typedef struct hashmap_node_s
{
  hashmap_key_t key;
  struct hashmap_node_s *next;
} hashmap_node_t;

/* must be placed at the beginning of the definition of a structure */
#define DS_HASHMAP_NODE() hashmap_node_t __hashmap0__;
/* Get the hashmap_node pointer safely */
#define hashmap_node(elem) ((hashmap_node_t*)(elem))
/* Get the element pointer from node pointer */
#define hashmap_entry(node, type) ((type *)(node))

typedef enum
{
  HASHMAP_KEY_INVALID = 0,
  HASHMAP_KEY_CHR,
  HASHMAP_KEY_INTPTR,
} hashmap_type_t;

typedef unsigned long (*pfnKeyHash)( hashmap_key_t key );
typedef bool (*pfnKeyComp)( hashmap_key_t src, hashmap_key_t dst );

typedef struct hashmap_s
{
  int               size;               /* count of bucket */
  int               element_count;
  hashmap_type_t    keytype;
  pfnKeyHash        hash_func;
  pfnKeyComp        comp_func;
  hashmap_node_t ** nodes;
  int               next_resize;
  float             max_load_factor;
  float             growth_factor;
  hashmap_node_t *  collected_root;    /* for waste collector */
  hashmap_node_t *  collected_tail;
} hashmap_t;

int hashmap_init( hashmap_t *hashmap, hashmap_type_t keytype, size_t init_size );
void hashmap_insert( hashmap_t *hashmap, hashmap_key_t key, hashmap_node_t *node );
hashmap_node_t *hashmap_at( hashmap_t *hashmap, hashmap_key_t key );
int hashmap_remove( hashmap_t *hashmap, hashmap_key_t key, pfn_free_elem free_elem );
void hashmap_clear( hashmap_t *hashmap, pfn_free_elem free_elem );
void hashmap_release( hashmap_t *hashmap, pfn_free_elem free_elem );

static inline bool
hashmap_contain_node( hashmap_t *hashmap, hashmap_key_t key, hashmap_node_t *node )
{
  const hashmap_node_t *orig = hashmap_at( hashmap, key );
  return( orig == node );
}

static inline bool
hashmap_contain_key( hashmap_t *hashmap, hashmap_key_t key )
{
  return( NULL!=hashmap_at( hashmap, key ) );
}

static bool inline
hashmap_empty( hashmap_t *hashmap )
{
  return hashmap->element_count ? false : true;
}

/*
 * Memory Allocator for hashmap
 */
#define hashmap_collect(hashmap, node) \
  do { \
      hashmap_node(node)->next = (hashmap)->collected_root; \
      (hashmap)->collected_root = hashmap_node(node); \
      } while(0)

#define hashmap_alloc_node(hashmap, type)\
  ({ \
      hashmap_node_t *_node_ = (hashmap)->collected_root; \
      if ( _node_ ) \
        (hashmap)->collected_root = _node_->next; \
      else \
        _node_ = (hashmap_node_t *)ds_heap_alloc(sizeof(type)); \
      (hashmap_entry(_node_, type)); \
    })

#define hashmap_cast_at(hashmap, key, type) ( hashmap_entry(hashmap_at( (hashmap), (key) ), type) )

/* Get element by its key, if the element is not existing, create a new one
 * if there is no memory, RETURN error code. */
#define hashmap_new_at(hashmap, key, type) \
  ({ \
      type *_tab_node = hashmap_cast_at( hashmap, key, type); \
      if ( !_tab_node ) \
        { \
          _tab_node = hashmap_alloc_node(hashmap, type); \
          if ( !_tab_node ) \
            return -DS_NO_MEMORY; \
          hashmap_insert( hashmap, key, hashmap_node(_tab_node) ); \
        } \
      (_tab_node); \
    })

/* Get element by its key, if the element is not existing, create a new on
 * if there is no memory, RETURN. */
#define hashmap_new_at2(hashmap, key, type) \
  ({ \
      type *_tab_node = hashmap_cast_at( hashmap, key, type); \
      if ( !_tab_node ) \
        { \
          _tab_node = hashmap_alloc_node(hashmap, type); \
          if ( !_tab_node ) \
            return; \
          hashmap_insert( hashmap, key, hashmap_node(_tab_node) ); \
        } \
      (_tab_node); \
    })

#define hashmap_max_load_factor(hashmap) ((hashmap)->max_load_factor)
#define hashmap_growth_factor(hashmap) ((hashmap)->growth_factor)

/* operation of buckets */
#define hashmap_bucket_size(hashmap) ((hashmap)->size)
#define hashmap_bucket(hashmap, bucket) ((hashmap)->nodes[bucket])
#define hashmap_bucket_next(node) ((node)->next)

/*
 * hashmap foreach
 *
 * note: never remove or free the node within foreach_hashmap() code block, unless
 * break; or return; after you did.
 *
 * @param _type Static Type of element pointer in the list, must be a pointer.
 * @param _valname The symbol name of variable
 * @param _list Pointer to the source list.
 */
#define foreach_hashmap(_type, _valname, _hashmap) \
  for(int _hashmap_bucket = 0; _hashmap_bucket< hashmap_bucket_size((_hashmap)); _hashmap_bucket++ ) \
    for( _type *_valname = hashmap_entry(hashmap_bucket((_hashmap), _hashmap_bucket), _type); \
        NULL!=_valname; \
        _valname = hashmap_entry(hashmap_bucket_next(hashmap_node(_valname)), _type))

END_C_DECLS

#endif //!defined(DSIM_HASHMAP_H_)
