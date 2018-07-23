/** @file
 * rbtree implement
 */

#include <dsim/rbtree.h>

/* rbtree properties:
 * 1. Any node is black or red
 * 2. The root is black
 * 3. Any leaf ( NULL node ) is black
 * 4. Both children of a red node are black
 * 5. Paths from a node to all its leaf children contains the same number of
 *    black nodes ( black depth )
 */

/* Private helper macros */
#define RB_BLACK	0
#define RB_RED		1
#define rb_color(node)		( ( size_t )( node )->_parent & 1 )
#define rb_is_black(node)	( node == NULL || rb_color( node ) == RB_BLACK )
#define rb_is_red(node)		( node && rb_color( node ) == RB_RED )
#define rb_parent(node)		( ( rb_node_t * )( ( node )->_parent & ~1 ) )
#define rb_set_color(node, color) \
  do { \
      ( node )->_parent = ( ( ( node )->_parent ) & ~1 ) | ( size_t )( color ); \
      } while (0)
#define rb_set_parent(node, parent)	\
  do { \
      ( node )->_parent = ( ( ( node )->_parent ) & 1 ) | ( size_t )( parent ); \
      } while (0)
#define rb_set_parent_and_color(node, parent, color) \
  do { \
      ( node )->_parent = ( ( size_t )( parent ) ) | ( size_t )( color ); \
      } while (0)

static inline
void rb_left_rotate( rb_tree_t *tree, rb_node_t *n )
{
  /*    p              n
   *   / \            / \
   *  x   n    ->    p   z
   *     / \        / \
   *    y   z      x   y
   */
  rb_node_t *p = rb_parent( n );
  rb_node_t *g = rb_parent( p );
  rb_set_parent( n, g );
  if ( g == NULL )
    tree->root = n;
  else
    {
      if (  p == g->left  )
        g->left = n;
      else
        g->right = n;
    }
  p->right = n->left;
  if ( p->right )
    rb_set_parent( p->right, p );
  n->left = p;
  rb_set_parent( p, n );
}

static inline
void rb_right_rotate( rb_tree_t *tree, rb_node_t *n )
{
  /*      p          n
   *     / \        / \
   *    n   z  ->  x   p
   *   / \            / \
   *  x   y          y   z
   */
  rb_node_t *p = rb_parent( n );
  rb_node_t *g = rb_parent( p );
  rb_set_parent( n, g );
  if ( g == NULL )
    tree->root = n;
  else
    {
      if ( p == g->left )
        g->left = n;
      else
        g->right = n;
    }
  p->left = n->right;
  if ( p->left )
    rb_set_parent( p->left, p );
  n->right = p;
  rb_set_parent( p, n );
}

static void
rb_exchange( rb_tree_t *tree, rb_node_t *victim, rb_node_t *replacement )
{
  rb_node_t *pv = rb_parent(  victim  );
  if ( pv == NULL )
    tree->root = replacement;
  else if (  pv->left == victim  )
    pv->left = replacement;
  else
    pv->right = replacement;
  rb_node_t *pr = rb_parent(  replacement  );
  if ( pr == NULL )
    tree->root = victim;
  else if ( pr->left == replacement )
    pr->left = victim;
  else
    pr->right = victim;

  size_t _p = victim->_parent;
  rb_node_t *l = victim->left;
  rb_node_t *r = victim->right;
  victim->_parent = replacement->_parent;
  victim->left = replacement->left;
  if ( victim->left )
    rb_set_parent( victim->left, victim );
  victim->right = replacement->right;
  if ( victim->right )
    rb_set_parent( victim->right, victim );
  replacement->_parent = _p;
  replacement->left = l;
  if ( replacement->left )
    rb_set_parent( replacement->left, replacement );
  replacement->right = r;
  if ( replacement->right )
    rb_set_parent( replacement->right, replacement );
}

static void
rb_add_fixup(  rb_tree_t *tree, rb_node_t *n  )
{
  rb_node_t *p = rb_parent( n ); /* parent */
  rb_node_t *g; /* grandparent */
  rb_node_t *u; /* uncle */

  for( ;; )
    {
      /* Loop invariant: n is red */
      if ( p == NULL )
        {
          rb_set_parent_and_color( n, NULL, RB_BLACK );
          break;
        }
      if ( rb_color( p ) == RB_BLACK )
        break;

      /* Since p is red, g must exists as the tree root must be black
       * g must also be black
       */
      g = rb_parent( p );
      if ( p == g->left )
        u = g->right;
      else
        u = g->left;
      if ( rb_is_red( u ) )
        {
          /* Case 1
          *      G           g
          *     / \         / \
          *    p   u  =>   P   U
          *   /           /
          *  n           n
          * ( also works for mirrored case )
          */
          rb_set_parent_and_color( p, g, RB_BLACK );
          rb_set_parent_and_color( u, g, RB_BLACK );
          n = g;
          p = rb_parent( n );
          rb_set_parent_and_color( n, p, RB_RED );
          continue;
        }

      if ( p == g->left )
        {
          if ( n == p->right )
            {
              /* Case 2
               *      G           G
               *     / \         / \
               *    p   U  =>   n   U
               *     \         /
               *      n       p
               */
              rb_left_rotate( tree, n );
              p = n;
              n = n->left;
              /* Continue to case 3 */
            }
          /* Case 3
           *      G           P
           *     / \         / \
           *    p   U  =>   n   g
           *   / \             / \
           *  n   X           X   U
           */
          rb_right_rotate( tree, p );
          rb_set_color( p, RB_BLACK );
          rb_set_parent_and_color( g, p, RB_RED );
          break;
        }
      else
        {
          /* Mirrored case 2 and 3 */
          if ( n == p->left )
            {
              rb_right_rotate( tree, n );
              p = n;
              n = n->right;
            }
          rb_left_rotate( tree, p );
          rb_set_color( p, RB_BLACK );
          rb_set_parent_and_color( g, p, RB_RED );
          break;
        }
    }
}

void
rb_add(  rb_tree_t *tree, rb_node_t *node, rb_cmp *cmp  )
{
  node->left = NULL;
  node->right = NULL;
  if ( tree->root == NULL )
    {
      tree->root = node;
      rb_set_parent_and_color( node, NULL, RB_BLACK );
      return;
    }

  rb_node_t *cur = tree->root;
  for( ;; )
    {
      int c = cmp( node, cur );
      if ( c < 0 )
        {
          if ( cur->left == NULL )
            {
              cur->left = node;
              rb_set_parent_and_color( node, cur, RB_RED );
              break;
            }
          cur = cur->left;
        }
      else
        {
          if ( cur->right == NULL )
            {
              cur->right = node;
              rb_set_parent_and_color( node, cur, RB_RED );
              break;
            }
          cur = cur->right;
        }
    }
  rb_add_fixup( tree, node );
}

static void
rb_remove_fixup(  rb_tree_t *tree, rb_node_t *n  )
{
  /* n is the replacement */
  rb_node_t *p = rb_parent( n ); /* parent */
  rb_node_t *s; /* sibling */
  rb_node_t *x, *y; /* children of sibling */
  for( ;; )
    {
      /* Loop invariant:
       * The black depth of n is one less than its sibling
       */
      if ( rb_color( n ) == RB_RED )
        {
          rb_set_color( n, RB_BLACK );
          break;
        }
      if ( p == NULL )
          break;
      if ( n == p->left )
        {
          s = p->right;
          /* s must exist
           * Otherwise the loop invariant is invalid
           */
          if ( rb_color( s ) == RB_RED )
            {
              /* Case 1
               *      P            S
               *     / \          / \
               *    N   s   =>   p   Y
               *       / \      / \
               *      X   Y    N   X
               */
              rb_left_rotate( tree, s );
              rb_set_color( s, RB_BLACK );
              rb_set_parent_and_color( p, s, RB_RED );
              s = p->right;
            }
          x = s->left;
          y = s->right;
          if ( rb_is_black( x ) && rb_is_black( y ) )
            {
              /* Case 2
               *     [p]          [p]
               *     / \          / \
               *    N   S   =>   N   s
               *       / \          / \
               *      X   Y        X   Y
               */
              rb_set_parent_and_color( s, p, RB_RED );
              n = p;
              p = rb_parent( n );
              continue;
            }
          if ( rb_is_black( y ) )
            {
              /* Case 3
               *     [p]          [p]
               *     / \          / \
               *    N   S   =>   N   X
               *       / \            \
               *      x   Y            s
               *       \              / \
               *        Z            Z   Y
               */
              rb_right_rotate( tree, x );
              rb_set_parent_and_color( x, p, RB_BLACK );
              rb_set_parent_and_color( s, x, RB_RED );
              y = s;
              s = x;
              x = x->left;
            }
          /* Case 4
           *     [p]          [s]
           *     / \          / \
           *    N   S   =>   P   Y
           *       / \      / \
           *     [x]  y    N  [x]
           */
          rb_left_rotate( tree, s );
          rb_set_color( s, rb_color( p ) );
          rb_set_parent_and_color( p, s, RB_BLACK );
          rb_set_parent_and_color( y, s, RB_BLACK );
          break;
        }
      else /* Mirrored cases */
        {
          s = p->left;
          if ( rb_color( s ) == RB_RED )
            {
              rb_right_rotate( tree, s );
              rb_set_color( s, RB_BLACK );
              rb_set_parent_and_color( p, s, RB_RED );
              s = p->left;
            }
          x = s->right;
          y = s->left;
          if ( rb_is_black( x ) && rb_is_black( y ) )
            {
              rb_set_parent_and_color( s, p, RB_RED );
              n = p;
              p = rb_parent( n );
              continue;
            }
          if ( rb_is_black( y ) )
            {
              rb_left_rotate( tree, x );
              rb_set_parent_and_color( x, p, RB_BLACK );
              rb_set_parent_and_color( s, x, RB_RED );
              y = s;
              s = x;
              x = x->right;
            }
          rb_right_rotate( tree, s );
          rb_set_color( s, rb_color( p ) );
          rb_set_parent_and_color( p, s, RB_BLACK );
          rb_set_parent_and_color( y, s, RB_BLACK );
          break;
        }
    }
}

void
rb_remove(  rb_tree_t *tree, rb_node_t *node  )
{
  if ( node->left && node->right )
    {
      /* Replace node with its successor and remove the successor instead */
      rb_node_t *next = rb_next( node );
      rb_exchange( tree, node, next );
      /* The successor must have at most one child, therefore no need for looping */
    }
  rb_node_t *p = rb_parent( node );
  rb_node_t *n = ( rb_node_t * )( ( size_t )node->left + ( size_t )node->right );
  if ( p == NULL )
    {
      tree->root = n;
      if ( n )
        rb_set_parent_and_color( n, NULL, RB_BLACK );
      return;
    }

  if ( rb_is_black( node ) ) /* Nothing to do if node is red */
    {
      if ( n == NULL )
        {
          /* Fake node as the replacement for fixup, remove it later */
          rb_remove_fixup( tree, node );
          if ( p->left == node )
            p->left = NULL;
          else
            p->right = NULL;
        }
      else
        {
          rb_set_parent( n, p );
          if ( p->left == node )
            p->left = n;
          else
            p->right = n;
          rb_remove_fixup( tree, n );
        }
    }
  else
    {
      if ( n )
        rb_set_parent( n, p );
      if ( p->left == node )
        p->left = n;
      else
        p->right = n;
    }
}

rb_node_t *
rb_find(  rb_tree_t *tree, const rb_node_t *value, rb_cmp *cmp  )
{
  rb_node_t *cur = tree->root;
  for( ;; )
    {
      if ( cur == NULL )
        return NULL;
      int r = cmp( value, cur );
      if ( r < 0 )
        cur = cur->left;
      else if ( r > 0 )
        cur = cur->right;
      else
        return cur;
    }
  return NULL; /* never reach here */
}

/* Find the smallest node in tree which is not less than value */
rb_node_t *
rb_lower_bound(  rb_tree_t *tree, const rb_node_t *value, rb_cmp *cmp  )
{
  rb_node_t *cur = tree->root;
  rb_node_t *candidate = NULL;
  for( ;; )
    {
      if ( cur == NULL )
        return candidate;
      int r = cmp( value, cur );
      if ( r < 0 )
        {
          candidate = cur;
          cur = cur->left;
        }
      else if ( r > 0 )
        cur = cur->right;
      else
        return cur;
    }
  return NULL; /* never reach here */
}

/* Find the largest node in tree which is not larger than value */
rb_node_t *
rb_upper_bound(  rb_tree_t *tree, const rb_node_t *value, rb_cmp *cmp  )
{
  rb_node_t *cur = tree->root;
  rb_node_t *candidate = NULL;
  for( ;; )
    {
      if ( cur == NULL )
        return candidate;
      int r = cmp( value, cur );
      if ( r < 0 )
        cur = cur->left;
      else if ( r > 0 )
        {
          candidate = cur;
          cur = cur->right;
        }
      else
        return cur;
    }
  return NULL; /* never reach here */
}

/* Get the first node of a tree, NULL if the tree is empty */
rb_node_t *
rb_first(  rb_tree_t *tree  )
{
  if ( rb_empty( tree ) )
    return NULL;
  rb_node_t *current = tree->root;
  while ( current->left != NULL )
    current = current->left;
  return current;
}

/* Get the last node of a tree, NULL if the tree is empty */
rb_node_t *
rb_last(  rb_tree_t *tree  )
{
  if ( rb_empty( tree ) )
    return NULL;
  rb_node_t *current = tree->root;
  while ( current->right != NULL )
    current = current->right;
  return current;
}

/* Get the precedent of a node, NULL if none */
rb_node_t *
rb_prev(  rb_node_t *node  )
{
  if ( node->left )
    {
      node = node->left;
      while ( node->right )
        node = node->right;
      return node;
    }

  rb_node_t *parent;
  while ( ( parent = rb_parent( node ) ) && parent && node == parent->left )
    node = parent;
  return parent;
}

/* Get the precedent of a node, NULL if none */
rb_node_t *
rb_next(  rb_node_t *node  )
{
  if ( node->right )
    {
      node = node->right;
      while ( node->left )
        node = node->left;
      return node;
    }

  rb_node_t *parent;
  while ( ( parent = rb_parent( node ) ) && parent && node == parent->right )
    node = parent;
  return parent;
}
