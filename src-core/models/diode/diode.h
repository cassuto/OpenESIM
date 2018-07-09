#ifndef DIODE_H_
#define DIODE_H_

#include "mdel-lib-internal.h"

typedef struct diode_param_s
{
  double resist;
  bool converged;
  double voltPN;
  double deltaV;
  double current;
  double threshold;
  double imped;
  double zenerV;
} diode_param_t;

int LIB_FUNC(diode_create)( circ_element_t * );
int LIB_FUNC(diode_init)( circ_element_t * );
int LIB_FUNC(diode_vchanged)( circ_element_t * );
int LIB_FUNC(diode_stamp)( circ_element_t * );
int LIB_FUNC(diode_config)( circ_element_t *, int, ... );
void LIB_FUNC(diode_uninit)( circ_element_t * );

static inline int
set_resist( diode_param_t *param, circ_element_t *element, double resist )
{
  if( resist == 0 ) resist = 0.1;
  param->imped = resist;
  return diode_vchanged( element );
}

#endif //!defined(DIODE_H_)
