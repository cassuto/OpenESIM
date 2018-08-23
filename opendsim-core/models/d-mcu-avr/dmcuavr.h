#ifndef DMCUAVR_H_
#define DMCUAVR_H_

#include "mdel-lib-internal.h"

#include "sim_avr.h"
#include "sim_irq.h"
#include "sim_elf.h"
#include "sim_hex.h"
#include "sim_core.h"
#include "avr_uart.h"
#include "avr_ioport.h"
#include "regmaps/regmaps.h"
#include "pinmaps/pinmaps.h"

typedef struct mmcu_s
{
  const char *      mmcu;
  const regmap_t *  regmap;
  int               pincount;
  const pinmap_t *  pinmap;
} mmcu_t;

typedef struct mcu_avr_param_s
{
  char *     mmcu;
  char *     romimg_fn;
  const mmcu_t *mcu;
  avr_t*     avr_processor;
  avr_irq_t* uart_in_irq;
  avr_cycle_count_t next_cycle;
  avr_cycle_count_t steps_pclk;
  avr_irq_t **port_irqs;
  avr_irq_t **ddr_irqs;
  avr_irq_t **wr_port_irqs;
  logic_state_t * state;
  double adc_in_imp;
  double dac_out_imp;
  double aref_in_imp;
} mcu_avr_param_t;

int LIB_FUNC(mcu_avr_create)( circ_element_t * );
int LIB_FUNC(mcu_avr_init)( circ_element_t * );
int LIB_FUNC(mcu_avr_stamp)( circ_element_t * );
int LIB_FUNC(mcu_avr_vchanged)( circ_element_t * );
int LIB_FUNC(mcu_avr_event)( circ_element_t * );
int LIB_FUNC(mcu_avr_config)( circ_element_t *, int, ... );
void LIB_FUNC(mcu_avr_reset)( circ_element_t *element );
void LIB_FUNC(mcu_avr_uninit)( circ_element_t * );
int LIB_FUNC(mcu_avr_clock)( circ_element_t * );


const mmcu_t *LIB_INT_FUNC(get_mmcu)( const char *mmcu );

#endif //!defined(DMCUAVR_H_)
