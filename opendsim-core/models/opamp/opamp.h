#ifndef OPAMP_H_
#define OPAMP_H_

#include "mdel-lib-internal.h"

typedef struct
{
  bool converged;
  double accuracy;
  double gain;
  double k;
  double voltPos;
  double voltNeg;
  double last_output;
  double last_input;
} opamp_param_t;

int LIB_FUNC(opamp_create)( circ_element_t * );
int LIB_FUNC(opamp_init)( circ_element_t * );
int LIB_FUNC(opamp_vchanged)( circ_element_t * );
int LIB_FUNC(opamp_config)( circ_element_t *, int, ... );
void LIB_FUNC(opamp_uninit)( circ_element_t * );

#endif //!defined(OPAMP_H_)
