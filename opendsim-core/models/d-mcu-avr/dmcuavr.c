/** @file
 * model-lib Digital Buffer gate Model
 */

/*
 *  OpenDSIM (A/D mixed circuit simulator)
 *  Copyleft (C) 2016, The first Middle School in Yongsheng Lijiang China
 *
 *  This project is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public License(GPL)
 *  as published by the Free Software Foundation; either version 2.1
 *  of the License, or (at your option) any later version.
 *
 *  This project is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 */

#include "dmcuavr.h"

adsim_descriptor_t adsim_mcu_avr =
  {
    .pfn_create = mcu_avr_create,       /* callback function to create parameters */

    .pfn_init = mcu_avr_init,           /* callback function to initialize the simulator model */

    .pfn_stamp = mcu_avr_stamp,         /* callback function to stamp node matrix */

    .pfn_vchanged = mcu_avr_vchanged,   /* callback function to inform volt changed */

    .pfn_config = mcu_avr_config,       /* callback function to configure the element */

    .pfn_reset = NULL,                  /* callback function to reset the status */

    .pfn_uninit = mcu_avr_uninit,       /* callback function to terminate the parameters */

    .pfn_clock = mcu_avr_clock,         /* callback function to inform clock signal triggered */

    .pfn_event = mcu_avr_event          /* callback function to inform volt changed */
  };

circ_element_descriptor_t mdel_mcu_avr =
  {
    .param_size = sizeof(mcu_avr_param_t),

    .symbol_name = "MCU_AVR",           /* symbol name used to identify this element */

    .pin_count = 28,                    /* alterable, the number of pins of device */

    .mdel_type = MDEL_AD,               /* the electrical type of this model */

    .mdel = &adsim_mcu_avr              /* pointer to the model descriptor structure */
  };
