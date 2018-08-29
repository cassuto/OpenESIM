#ifndef D_LOGICSTATE_H_
#define D_LOGICSTATE_H_

#include "mdel-lib-internal.h"

typedef struct logicstate_param_s
{
  logic_state_t *state;
  int curpin;
} logicstate_param_t;

int LIB_FUNC(logicstate_create)( circ_element_t * );
int LIB_FUNC(logicstate_init)( circ_element_t * );
int LIB_FUNC(logicstate_event)( circ_element_t * );
int LIB_FUNC(logicstate_config)( circ_element_t *, int, ... );
void LIB_FUNC(logicstate_uninit)( circ_element_t * );

#endif //!defined(D_LOGICSTATE_H_)
