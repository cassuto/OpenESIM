#ifndef D_FLIPFLOP_JK_H_
#define D_FLIPFLOP_JK_H_

#include "mdel-lib-internal.h"

typedef struct flipflop_jk_paras
{
  bool clk;
} flipflop_jk_param_t;

int LIB_FUNC(flipflop_jk_create)( circ_element_t * );
int LIB_FUNC(flipflop_jk_init)( circ_element_t * );
int LIB_FUNC(flipflop_jk_event)( circ_element_t * );
int LIB_FUNC(flipflop_jk_config)( circ_element_t *, int, ... );

#endif //!defined(D_FLIPFLOP_JK_H_)
