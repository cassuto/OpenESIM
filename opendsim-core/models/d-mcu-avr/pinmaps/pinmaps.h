#ifndef MCU_AVR_PINMAPS_H_
#define MCU_AVR_PINMAPS_H_

typedef struct pinmap_s
{
  char bank;
  int bankid;
} pinmap_t;

#define PIN_VCC (1)
#define PIN_GND (2)
#define PIN_AVCC (3)
#define PIN_AREF (4)
#define PIN_END (32)

#endif
