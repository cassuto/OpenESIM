#ifndef D_DRVSRC_H_
#define D_DRVSRC_H_

#include "mdel-lib-internal.h"

typedef struct drvsrc_paras
{
  logic_state_t state;
} drvsrc_param_t;

int LIB_FUNC(drvsrc_create)( circ_element_t * );
int LIB_FUNC(drvsrc_init)( circ_element_t * );
int LIB_FUNC(drvsrc_event)( circ_element_t * );
int LIB_FUNC(drvsrc_config)( circ_element_t *, int, ... );
int LIB_FUNC(drvsrc_clock)( circ_element_t *element );

#endif //!defined(D_DRVSRC_H_)
