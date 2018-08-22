#ifndef PINMAPS_ATMEGA64_H_
#define PINMAPS_ATMEGA64_H_

#ifndef MCU_AVR_REGMAPS_H_
#include "pinmaps.h"
#endif

static const pinmap_t pinmaps_atmega164[] =
{
  /* 1   */ {'C', 6 },
  /* 2   */ {'D', 0 },
  /* 3   */ {'D', 1 },
  /* 4   */ {'D', 2 },
  /* 5   */ {'D', 3 },
  /* 6   */ {'D', 4 },
  /* 7   */ {'X', -PIN_VCC },
  /* 8   */ {'X', -PIN_GND },
  /* 9   */ {'B', 6 },
  /* 10  */ {'B', 7 },
  /* 11  */ {'D', 5 },
  /* 12  */ {'D', 6 },
  /* 13  */ {'D', 7 },
  /* 14  */ {'B', 0 },
  /* 15  */ {'B', 1 },
  /* 16  */ {'B', 2 },
  /* 17  */ {'B', 3 },
  /* 18  */ {'B', 4 },
  /* 19  */ {'B', 5 },
  /* 20  */ {'X', -PIN_AVCC },
  /* 21  */ {'X', -PIN_AREF },
  /* 22  */ {'X', -PIN_GND },
  /* 23  */ {'C', 0 },
  /* 24  */ {'C', 1 },
  /* 25  */ {'C', 2 },
  /* 26  */ {'C', 3 },
  /* 27  */ {'C', 4 },
  /* 28  */ {'C', 5 },
  {'E', -PIN_END }
};

#endif
