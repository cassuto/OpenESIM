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

#ifndef DSIM_LIST_H_
#define DSIM_LIST_H_

#include <dsim/types.h>
#include <dsim/cdecl.h>

C_DECLS

typedef struct ds_list_node_s
{
  struct ds_list_node_s *next;
  struct ds_list_node_s *prev;
} list_node_t;

typedef struct list_s
{
  list_node_t *root, *tail;
  list_node_t *collected_root; /* for waste collector */
} list_t;

/* must be placed at the beginning of the definition of a structure */
#define DS_LIST_NODE() list_node_t _list_node_;
/* Get the node pointer from element pointer */
#define list_node(elem) ((list_node_t *)(elem))
/* Get the element pointer from node pointer */
#define list_entry(node, type) ((type *)(node))

/* Get the element pointer from node pointer */
#define list_entry(node, type) ((type *)(node))

/* Test if the list is empty */
#define list_empty(lst)  ((lst)->root == NULL)

/* Initialize a list */
#define list_init(list) \
  do { \
    (list)->root = (list)->tail = NULL; \
    (list)->collected_root = NULL; \
    } while(0)

/* add an element to list. Be sure that the new node must not be existing in the list */
static inline void
list_insert( list_t *list, list_node_t *node )
{
  node->prev = list->tail;
  node->next = NULL;

  if( LIKELY( list->tail ) )
    list->tail->next = node;
  else
    list->root = node; /* set root */
  list->tail = node; /* update tail */
}

/* remove the specified element from list */
static inline void
list_remove( list_t *list, list_node_t *node )
{
  if ( UNLIKELY( list->root == node ) )
    list->root = node->next;
  if ( UNLIKELY( list->tail == node ) )
    list->tail = node->prev;

  if ( node->prev )
    node->prev->next = node->next;
  if ( node->next )
    node->next->prev = node->prev;
}

static inline bool
list_contains( const list_t *list, const list_node_t *src_node )
{
  list_node_t *node = list->root;
  while ( node )
    {
      if ( node == src_node )
        return true;
      node = node->next;
    }

  return false;
}

static inline int
list_size( const list_t *list )
{
  register int count = 0;
  list_node_t *node = list->root;

  while ( node )
    {
      count++;
      node = node->next;
    }

  return count;
}

void list_clear( list_t *list, pfn_free_elem free_elem );
int list_copy( list_t *dst, const list_t *src, pfn_free_elem free_elem, pfn_copy_elem copy );
void list_release( list_t *list, pfn_free_elem free_elem );

/*
 * Memory Allocator for list
 */
#define list_collect(list, node) \
  do { \
    list_node(node)->next = (list)->collected_root; \
    (list)->collected_root = list_node(node); \
    } while(0)

#define list_alloc_node(list, type) \
  ({ \
      list_node_t *_node_ = (list)->collected_root; \
      if ( _node_ ) \
        (list)->collected_root = _node_->next; \
      else \
        _node_ = (list_node_t *)ds_heap_alloc(sizeof(type)); \
      (list_entry(_node_, type)); \
    })

#define list_next(node) ((node)->next)
#define list_prev(node) ((node)->prev)

/*
 * chain list foreach
 *
 * note: never remove or free the node within foreach_list() code block, unless
 * break; or return; after you did.
 *
 * @param _type Static Type of element pointer in the list, must be a pointer.
 * @param _valname The symbol name of variable
 * @param _list Pointer to the source list.
 */
#define foreach_list(_type, _valname, _list) \
  for( _type *_valname = list_entry((_list)->root, _type); \
      NULL!=_valname; \
      _valname = list_entry(list_next(list_node(_valname)), _type))

/*
 * chain list reversed foreach
 *
 * note: never remove or free the node within foreach_list() code block, unless
 * break; or return; after you did.
 *
 * @param _type Static Type of element pointer in the list, must be a pointer.
 * @param _valname The symbol name of variable
 * @param _list Pointer to the source list.
 */
#define foreach_reversed_list(_type, _valname, _list) \
  for( _type *_valname = list_entry((_list)->tail, _type); \
      NULL!=_valname; \
      _valname = list_entry(list_prev(list_node(_valname)), _type))

END_C_DECLS

#endif //!defined(DSIM_LIST_H_)
