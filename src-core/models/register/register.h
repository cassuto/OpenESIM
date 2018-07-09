#ifndef ADC_H_
#define ADC_H_

#include "mdel-lib-internal.h"

typedef struct register_param_s
{
  double resist;
  double current;
} register_param_t;

int LIB_FUNC(register_create)( circ_element_t * );
int LIB_FUNC(register_init)( circ_element_t * );
int LIB_FUNC(register_stamp)( circ_element_t * );
int LIB_FUNC(register_config)( circ_element_t *, int, ... );
void LIB_FUNC(register_uninit)( circ_element_t * );

#endif //!defined(ADC_H_)
