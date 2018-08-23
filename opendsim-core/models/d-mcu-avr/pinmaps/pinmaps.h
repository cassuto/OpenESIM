#ifndef MCU_AVR_PINMAPS_H_
#define MCU_AVR_PINMAPS_H_

typedef struct pinmap_s
{
  const char bank;
  const int bankid;
  const int typemask;
} pinmap_t;

#define PIN_IO (1)
#define PIN_VCC (1 << 1)
#define PIN_GND (1 << 2)
#define PIN_AVCC (1 << 3)
#define PIN_AREF (1 << 4)
#define PIN_ADC (1 << 5)
#define PIN_DAC (1 << 6)
#define PIN_RESET (1 << 7)
#define PIN_END (1 << 16)

#endif
