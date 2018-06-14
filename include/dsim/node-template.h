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

#ifndef DSIM_NODE_TEMPLATE_H_
#define DSIM_NODE_TEMPLATE_H_

#include <dsim/types.h>
#include <dsim/error.h>
#include <dsim/memory.h>
#include <dsim/list.h>
#include <dsim/hashmap.h>

/*
 * Template functions for hashmap
 */

#define DEFINE_NODE_TEMPLATE(container, type, name) \
  typedef struct \
  { \
    DS_ ## container ##_NODE(); \
    type val; \
  } name;

DEFINE_NODE_TEMPLATE(HASHMAP, double, hashmap_double_node_t)
DEFINE_NODE_TEMPLATE(HASHMAP, int, hashmap_int_node_t)
DEFINE_NODE_TEMPLATE(LIST, int, list_int_node_t)
DEFINE_NODE_TEMPLATE(LIST, void*, list_voidptr_node_t)

/*
 * Template functions for list
 */

#define DEFINE_LIST_DUP_TEMPLATE(type, name) \
  static void* \
  name( const void *src ) \
  { \
    type *node = (type *)ds_heap_alloc(sizeof(*node)); \
    *node = *((type *)src); \
    return node; \
  }

DEFINE_LIST_DUP_TEMPLATE(list_int_node_t, list_int_dup)
DEFINE_LIST_DUP_TEMPLATE(list_voidptr_node_t, list_voidptr_dup)

#define DEFINE_LIST_CONTAINS_TEMPLATE(type, type_name) \
  static bool \
  list_contains_## type_name( const list_t *list, type src ) \
  { \
    foreach_list( list_## type_name ##_node_t, node, list ) \
      { \
        if ( node->val == src ) \
          return true; \
      } \
    return false; \
  }

DEFINE_LIST_CONTAINS_TEMPLATE(void*, voidptr)
DEFINE_LIST_CONTAINS_TEMPLATE(int, int)

#define DEFINE_LIST_INSERT_TEMPLATE(type, type_name) \
  static int \
  list_insert_## type_name ( list_t *list, type src ) \
  { \
    list_ ## type_name ## _node_t *node = (list_ ## type_name ## _node_t *)ds_heap_alloc(sizeof(*node)); \
    if ( node ) \
      { \
        node->val = src; \
        list_insert( list, list_node(node) ); \
        return 0; \
      } \
    return -DS_NO_MEMORY; \
  }

DEFINE_LIST_INSERT_TEMPLATE(void*, voidptr)
DEFINE_LIST_INSERT_TEMPLATE(int, int)

#endif//!defined(DSIM_NODE_TEMPLATE_H_)
