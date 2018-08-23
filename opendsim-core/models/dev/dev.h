#ifndef DEV_H_
#define DEV_H_

#include <dsim/cdecl.h>

#include "mdel-lib-internal.h"

C_DECLS

typedef struct
{
  void *pIDevice;
} adev_param_t;

typedef struct
{
  void *pIDevice;
} ddev_param_t;

int LIB_FUNC(adev_create)( circ_element_t * );
int LIB_FUNC(adev_init)( circ_element_t * );
int LIB_FUNC(adev_stamp)( circ_element_t * );
int LIB_FUNC(adev_vchanged)( circ_element_t * );
void LIB_FUNC(adev_reset)( circ_element_t * );
int LIB_FUNC(adev_config)( circ_element_t *, int, ... );
void LIB_FUNC(adev_uninit)( circ_element_t * );

int LIB_FUNC(ddev_create)( circ_element_t * );
int LIB_FUNC(ddev_init)( circ_element_t * );
void LIB_FUNC(ddev_reset)( circ_element_t * );
int LIB_FUNC(ddev_config)( circ_element_t *, int, ... );
void LIB_FUNC(ddev_uninit)( circ_element_t * );
int LIB_FUNC(ddev_clock)( circ_element_t * );
int LIB_FUNC(ddev_event)( circ_element_t * );

END_C_DECLS

#endif //!defined(DEV_H_)
