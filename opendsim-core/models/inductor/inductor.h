#ifndef INDUCTOR_H_
#define INDUCTOR_H_

#include "mdel-lib-internal.h"

typedef struct inductor_param_s
{
  double resist;
  double ind;
  double current_source;
  double dT;
  double volt;
} inductor_param_t;

int LIB_FUNC(inductor_create)( circ_element_t * );
int LIB_FUNC(inductor_init)( circ_element_t * );
int LIB_FUNC(inductor_stamp)( circ_element_t * );
int LIB_FUNC(inductor_vchanged)( circ_element_t *element );
int LIB_FUNC(inductor_config)( circ_element_t *, int, ... );
void LIB_FUNC(inductor_uninit)( circ_element_t * );

static inline int
set_resist( circ_element_t *element, inductor_param_t *param, double value )
{
  if( value<1e-9 ) value = 1e-9;
  param->resist = value;
  return inductor_stamp( element );
}

#endif //!defined(INDUCTOR_H_)
