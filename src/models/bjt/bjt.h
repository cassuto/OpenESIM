#ifndef BJT_H_
#define BJT_H_

#include "mdel-lib-internal.h"

typedef struct
{
  double resist;
  double accuracy;
  double last_out;
  double base_curr;
  double voltE;
  double voltPN;
  circ_element_t *BE_juct;
  bool E_follow;
  int gain;
  bool is_PNP;
} bjt_param_t;

int LIB_FUNC(bjt_create)( circ_element_t * );
int LIB_FUNC(bjt_init)( circ_element_t * );
int LIB_FUNC(bjt_stamp)( circ_element_t * );
int LIB_FUNC(bjt_vchanged)( circ_element_t * );
int LIB_FUNC(bjt_config)( circ_element_t *, int, ... );
void LIB_FUNC(bjt_uninit)( circ_element_t * );

static inline int
set_resist( circ_element_t *element, bjt_param_t *param, double resist )
{
  param->resist = resist;
  return bjt_stamp( element );
}

#endif //!defined(BJT_H_)
