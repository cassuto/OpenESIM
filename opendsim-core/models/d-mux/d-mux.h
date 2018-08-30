#ifndef D_MUX_H_
#define D_MUX_H_

#include "mdel-lib-internal.h"

typedef struct mux_paras
{
} mux_param_t;

int LIB_FUNC(mux_create)( circ_element_t * );
int LIB_FUNC(mux_init)( circ_element_t * );
int LIB_FUNC(mux_event)( circ_element_t * );
int LIB_FUNC(mux_config)( circ_element_t *, int, ... );

#endif //!defined(D_MUX_H_)
