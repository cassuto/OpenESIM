
#include "dmcuavr.h"

#include "regmaps/atmega164.h"
#include "pinmaps/atmega164.h"

static const mmcu_t mmcus[] =
{
  {"atmega164", regmaps_atmega164, 28, pinmaps_atmega164 },
  {NULL, NULL, -1, NULL}
};

const mmcu_t *
LIB_INT_FUNC(get_mmcu)( const char *mmcu )
{
  for( int i=0; mmcus[i].mmcu; i++ )
    {
      if( 0==strcmp( mmcus[i].mmcu, mmcu ) )
        {
          return &mmcus[i];
        }
    }
  return NULL;
}
