#ifndef D_BUFF_H_
#define D_BUFF_H_

#include "mdel-lib-internal.h"

typedef struct buff_param_s
{
  bool reversed;
} buff_param_t;

int LIB_FUNC(buff_create)( circ_element_t * );
int LIB_FUNC(buff_init)( circ_element_t * );
int LIB_FUNC(buff_event)( circ_element_t * );
int LIB_FUNC(buff_config)( circ_element_t *, int, ... );
void LIB_FUNC(buff_uninit)( circ_element_t * );

#endif //!defined(D_BUFF_H_)
