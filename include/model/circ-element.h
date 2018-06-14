#ifndef CIRC_ELEMENT_H_
#define CIRC_ELEMENT_H_

#include <dsim/list.h>
#include <model/circ-pin.h>

typedef struct circ_element_s
{
  DS_LIST_NODE();

  circ_pin_t **pin_vector;
  int pin_count;

} circ_element_t;

circ_element_t *circ_element_create( void );
int circ_element_set_pin_count( circ_element_t *element, int n );

#endif//!defined(CIRC_ELEMENT_H_)
