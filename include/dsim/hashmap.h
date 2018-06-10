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

#ifndef DSIM_HASHMAP_H_
#define DSIM_HASHMAP_H_

#include <dsim/types.h>
#include <dsim/list.h>

typedef uintptr_t hashmap_key_t;

typedef struct hashmap_node_s
{
  hashmap_key_t key;
  struct hashmap_node_s *prev;
} hashmap_node_t;

/* must be placed at the beginning of the definition of a structure */
#define DS_HASHMAP_NODE() hashmap_node_t __hashmap0__;
/* Get the list_node pointer safely */
#define DS_HASHMAP_GET(_elem) ((hashmap_node_t*)_elem)

typedef enum
{
  HASHMAP_KEY_INVALID = 0,
  HASHMAP_KEY_CHR,
  HASHMAP_KEY_INT,
} hashmap_type_t;

typedef unsigned long (*pfnKeyHash)( hashmap_key_t key );
typedef bool (*pfnKeyComp)( hashmap_key_t src, hashmap_key_t dst );

typedef void (pfnFreeElem)( void *elem );

typedef struct hashmap_s
{
  int size;
  hashmap_type_t keytype;
  pfnKeyHash hash_func;
  pfnKeyComp comp_func;
  hashmap_node_t **nodes;
} hashmap_t;

hashmap_t * hashmap_create( hashmap_type_t keytype, size_t size );
void hashmap_insert( hashmap_t *hashmap, hashmap_key_t key, void *node );
void *hashmap_at( hashmap_t *hashmap, hashmap_key_t key );
bool hashmap_contain_node( hashmap_t *hashmap, hashmap_key_t key, void *node );
bool hashmap_contain_key( hashmap_t *hashmap, hashmap_key_t key );
int hashmap_remove( hashmap_t *hashmap, hashmap_key_t key, pfnFreeElem free_elem );
void hashmap_free( hashmap_t *hashmap, pfnFreeElem free_elem );

#endif //!defined(DSIM_HASHMAP_H_)
