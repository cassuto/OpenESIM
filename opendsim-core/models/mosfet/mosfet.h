#ifndef MOSFET_H_
#define MOSFET_H_

#include "mdel-lib-internal.h"

typedef struct mosfet_param_s
{
  double resist;
  double accuracy;
  double last_current;
  double threshold;
  double kRDSon;
  double RDSon;
  double gateV;
  double Gth;
  double Vs;

  bool Pchannel;
  bool S_follow;
} mosfet_param_t;

int LIB_FUNC(mosfet_create)( circ_element_t * );
int LIB_FUNC(mosfet_init)( circ_element_t * );
int LIB_FUNC(mosfet_stamp)( circ_element_t * );
int LIB_FUNC(mosfet_vchanged)( circ_element_t * );
int LIB_FUNC(mosfet_config)( circ_element_t *, int, ... );
void LIB_FUNC(mosfet_uninit)( circ_element_t * );

static inline int
set_resist( circ_element_t *element, mosfet_param_t *param, double resist )
{
  param->resist = resist;
  return mosfet_stamp( element );
}

#endif //!defined(ADC_H_)
