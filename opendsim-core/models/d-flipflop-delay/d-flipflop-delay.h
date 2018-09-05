#ifndef D_FLIPFLOP_DELAY_H_
#define D_FLIPFLOP_DELAY_H_

#include "mdel-lib-internal.h"

typedef struct flipflop_delay_paras
{
  bool clk;
} flipflop_delay_param_t;

int LIB_FUNC(flipflop_delay_create)( circ_element_t * );
int LIB_FUNC(flipflop_delay_init)( circ_element_t * );
int LIB_FUNC(flipflop_delay_event)( circ_element_t * );
int LIB_FUNC(flipflop_delay_config)( circ_element_t *, int, ... );

#endif //!defined(D_FLIPFLOP_DELAY_H_)
