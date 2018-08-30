#ifndef D_DEMUX_H_
#define D_DEMUX_H_

#include "mdel-lib-internal.h"

typedef struct demux_paras
{
} demux_param_t;

int LIB_FUNC(demux_create)( circ_element_t * );
int LIB_FUNC(demux_init)( circ_element_t * );
int LIB_FUNC(demux_event)( circ_element_t * );
int LIB_FUNC(demux_config)( circ_element_t *, int, ... );

#endif //!defined(D_DEMUX_H_)
