#ifndef PINMAPS_ATMEGA64_H_
#define PINMAPS_ATMEGA64_H_

#ifndef MCU_AVR_REGMAPS_H_
#include "pinmaps.h"
#endif

static const pinmap_t pinmaps_atmega164[] =
{
  /* 1   */ {'C', 6, PIN_IO },
  /* 2   */ {'D', 0, PIN_IO },
  /* 3   */ {'D', 1, PIN_IO },
  /* 4   */ {'D', 2, PIN_IO },
  /* 5   */ {'D', 3, PIN_IO },
  /* 6   */ {'D', 4, PIN_IO },
  /* 7   */ {'X', -1, PIN_VCC },
  /* 8   */ {'X', -1, PIN_GND },
  /* 9   */ {'B', 6, PIN_IO },
  /* 10  */ {'B', 7, PIN_IO },
  /* 11  */ {'D', 5, PIN_IO },
  /* 12  */ {'D', 6, PIN_IO },
  /* 13  */ {'D', 7, PIN_IO },
  /* 14  */ {'B', 0, PIN_IO },
  /* 15  */ {'B', 1, PIN_IO },
  /* 16  */ {'B', 2, PIN_IO },
  /* 17  */ {'B', 3, PIN_IO },
  /* 18  */ {'B', 4, PIN_IO },
  /* 19  */ {'B', 5, PIN_IO },
  /* 20  */ {'X', -1, PIN_AVCC },
  /* 21  */ {'X', -1, PIN_AREF },
  /* 22  */ {'X', -1, PIN_GND },
  /* 23  */ {'C', 0, PIN_IO | PIN_ADC },
  /* 24  */ {'C', 1, PIN_IO | PIN_ADC },
  /* 25  */ {'C', 2, PIN_IO | PIN_ADC },
  /* 26  */ {'C', 3, PIN_IO | PIN_ADC },
  /* 27  */ {'C', 4, PIN_IO | PIN_ADC },
  /* 28  */ {'C', 5, PIN_IO | PIN_ADC },
  {'X', -1, PIN_END }
};

#endif
