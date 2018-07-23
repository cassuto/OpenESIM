#ifndef ADC_H_
#define ADC_H_

#include "mdel-lib-internal.h"

typedef struct
{
  int inputs_count;
  double input_vh, input_vl;
  double imp;
  double *volt;
  bool *state;
  bool *inverted;
  circ_node_t *vsrc_node;
} adc_param_t;

int LIB_FUNC(adc_create)( circ_element_t * );
int LIB_FUNC(adc_init)( circ_element_t * );
int LIB_FUNC(adc_stamp)( circ_element_t * );
int LIB_FUNC(adc_vchanged)( circ_element_t * );
int LIB_FUNC(adc_config)( circ_element_t *, int, ... );
void LIB_FUNC(adc_uninit)( circ_element_t * );

#endif //!defined(ADC_H_)
