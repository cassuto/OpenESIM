#ifndef D_LCD8544_H_
#define D_LCD8544_H_

#include "mdel-lib-internal.h"

typedef struct lcd8544_paras
{
  logic_state_t *state;
  bool dc;
  unsigned char gdram[6][84];  /* 84x48 GDRAM */
  bool bPD;
  bool bV;
  bool bH;
  bool bD;
  bool bE;
  bool lastScl;
  int addrX; /* X RAM address */
  int addrY; /* Y RAM address */
  int inBit; /* How many bits have we read since last byte */
  unsigned char cinBuf; /* Buffer where we keep incoming bits */
} lcd8544_param_t;

int LIB_FUNC(lcd8544_create)( circ_element_t * );
int LIB_FUNC(lcd8544_init)( circ_element_t * );
int LIB_FUNC(lcd8544_event)( circ_element_t * );
int LIB_FUNC(lcd8544_config)( circ_element_t *, int, ... );
void LIB_FUNC(lcd8544_reset)( circ_element_t *element );
void LIB_FUNC(lcd8544_uninit)( circ_element_t * );

void LIB_INT_FUNC(lcd8544_clear)( lcd8544_param_t *param );

#endif //!defined(D_LCD8544_H_)
