#ifndef VSOURCE_H_
#define VSOURCE_H_

#include "mdel-lib-internal.h"

typedef struct vsource_param_s
{
  double vh;
  double vl;
  double volt_out;
  double imp;
  double admit;
  bool out;
  circ_node_t *src_node;
} vsource_param_t;

int LIB_FUNC(vsource_create)( circ_element_t * );
int LIB_FUNC(vsource_init)( circ_element_t * );
int LIB_FUNC(vsource_stamp)( circ_element_t * );
int LIB_FUNC(vsource_config)( circ_element_t *, int, ... );
void LIB_FUNC(vsource_uninit)( circ_element_t * );

int LIB_INT_FUNC(vsource_stamp_output)( circ_element_t *element );

#endif //!defined(VSOURCE_H_)
