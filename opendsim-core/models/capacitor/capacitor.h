#ifndef CAPACITOR_H_
#define CAPACITOR_H_

#include "mdel-lib-internal.h"

typedef struct capacitor_param_s
{
  double cap;
  double resist;
  double dT;
} capacitor_param_t;

int LIB_FUNC(capacitor_create)( circ_element_t * );
int LIB_FUNC(capacitor_init)( circ_element_t * );
int LIB_FUNC(capacitor_stamp)( circ_element_t * );
int LIB_FUNC(capacitor_vchanged)( circ_element_t * );
int LIB_FUNC(capacitor_config)( circ_element_t *, int, ... );
void LIB_FUNC(capacitor_uninit)( circ_element_t * );

static inline int
set_resist( circ_element_t *element, capacitor_param_t *param, double resist )
{
  param->resist = resist;
  return capacitor_stamp( element );
}

#endif //!defined(CAPACITOR_H_)
