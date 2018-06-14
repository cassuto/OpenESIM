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

#ifndef DSIM_RBTREE_H_
#define DSIM_RBTREE_H_

#include <dsim/misc.h>
#include <dsim/types.h>

typedef struct rb_node_s
{
  size_t _parent;
  struct rb_node_s *left, *right;
} rb_node_t;

/* must be placed at the beginning of the definition of a structure */
#define DS_RBTREE_NODE() rb_node_t __rbtree0__;
/* Get the rb_node pointer safely */
#define rb_node(elem) ((rb_node_t*)(elem))
/* Get the element pointer from node pointer */
#define rb_entry(node, type) ((type *)(node))

typedef struct rb_tree_s
{
  rb_node_t *root;
} rb_tree_t;

typedef int rb_cmp(const rb_node_t *left, const rb_node_t *right);

/* Test if the tree is empty */
#define rb_empty(tree)	((tree)->root == NULL)

/* Initialize a tree */
#define rb_init(tree)	\
  do { \
    (tree)->root = NULL; \
    } while (0)

void rb_add( rb_tree_t *tree, rb_node_t *node, rb_cmp *cmp );
void rb_remove( rb_tree_t *tree, rb_node_t *node );
rb_node_t *rb_find( rb_tree_t *tree, const rb_node_t *value, rb_cmp *cmp );
rb_node_t *rb_lower_bound( rb_tree_t *tree, const rb_node_t *value, rb_cmp *cmp );
rb_node_t *rb_upper_bound( rb_tree_t *tree, const rb_node_t *value, rb_cmp *cmp );
rb_node_t *rb_first( rb_tree_t *tree );
rb_node_t *rb_last( rb_tree_t *tree );
rb_node_t *rb_prev( rb_node_t *node );
rb_node_t *rb_next( rb_node_t *node );


/*
 * rbtree foreach
 *
 * note: never remove or free the node within foreach_list() code block.
 *
 * @param _type Static Type of element pointer in the list, must be a pointer.
 * @param _valname The symbol name of variable
 * @param _list Pointer to the source list.
 */
#define foreach_rbtree(_type, _valname, _tree) \
  for( _type *_valname = rb_entry(rb_first(_tree), _type); \
      NULL!=_valname; \
      _valname = rb_entry(rb_next(rb_node(_valname)), _type))


#endif //!defined(DSIM_RBTREE_H_)
