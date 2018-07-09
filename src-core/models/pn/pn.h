#ifndef PN_H_
#define PN_H_

#include "mdel-lib-internal.h"

typedef struct pn_param_s
{
  double resist;
  double accuracy;
  double voltPN;
  double deltaV;
  double threshold;
} pn_param_t;

int LIB_FUNC(pn_create)( circ_element_t * );
int LIB_FUNC(pn_init)( circ_element_t * );
int LIB_FUNC(pn_stamp)( circ_element_t * );
int LIB_FUNC(pn_vchanged)( circ_element_t * );
int LIB_FUNC(pn_config)( circ_element_t *, int, ... );
void LIB_FUNC(pn_uninit)( circ_element_t * );

double LIB_INT_FUNC(pn_current)( circ_element_t *element );

#endif //!defined(PN_H_)
