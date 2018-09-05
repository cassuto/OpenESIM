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

/*
 * PIN Mapping:
 *
 *  0:      SET
 *  1:      RST
 *  2:      CLK
 *  3:      D
 *  4:      Q
 *  5:      ~Q
 */

#include "d-flipflop-delay.h"

dsim_descriptor_t dsim_flipflop_delay =
  {
    .pfn_create = flipflop_delay_create, /* callback function to create parameters */

    .pfn_init = flipflop_delay_init,     /* callback function to initialize the simulator model */

    .pfn_config = flipflop_delay_config, /* callback function to configure the element */

    .pfn_reset = NULL,                  /* callback function to reset the status */

    .pfn_uninit = NULL,                 /* callback function to terminate the parameters */

    .pfn_clock = NULL,                  /* callback function to inform clock signal triggered */

    .pfn_event = flipflop_delay_event    /* callback function to inform volt changed */
  };

circ_element_descriptor_t mdel_flipflop_delay =
  {
    .param_size = sizeof(flipflop_delay_param_t),

    .symbol_name = "DELAY-FLIP-FLOP",   /* symbol name used to identify this element */

    .pin_count = 6,                     /* alterable, the number of pins of device */

    .mdel_type = MDEL_DIGITAL,          /* the electrical type of this model */

    .mdel = &dsim_flipflop_delay        /* pointer to the model descriptor structure */
  };
