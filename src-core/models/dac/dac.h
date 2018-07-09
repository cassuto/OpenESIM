#ifndef DAC_H_
#define DAC_H_

#include "mdel-lib-internal.h"

typedef struct dac_param_s
{
  int outputs_count;
  double output_vh, output_vl;
  double imp;
  double *volt;
  bool *state;
  bool *inverted;
  circ_node_t **vsrc_node;
} dac_param_t;

int LIB_FUNC(dac_create)( circ_element_t * );
int LIB_FUNC(dac_init)( circ_element_t * );
int LIB_FUNC(dac_stamp)( circ_element_t * );
int LIB_FUNC(dac_event)( circ_element_t * );
int LIB_FUNC(dac_config)( circ_element_t *, int, ... );
void LIB_FUNC(dac_uninit)( circ_element_t * );

int LIB_INT_FUNC(dac_set_vsrcs)( circ_element_t *element, dac_param_t *param, int n );

#endif //!defined(DAC_H_)
