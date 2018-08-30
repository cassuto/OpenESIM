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
 *  0~7:    D0~D7
 *  8~10:   S0~S2
 *  11:     OE
 *  12:     DI
 */

#include "d-demux.h"

dsim_descriptor_t dsim_demux =
  {
    .pfn_create = demux_create,         /* callback function to create parameters */

    .pfn_init = demux_init,             /* callback function to initialize the simulator model */

    .pfn_config = demux_config,         /* callback function to configure the element */

    .pfn_reset = NULL,                  /* callback function to reset the status */

    .pfn_uninit = NULL,                 /* callback function to terminate the parameters */

    .pfn_clock = NULL,                  /* callback function to inform clock signal triggered */

    .pfn_event = demux_event            /* callback function to inform volt changed */
  };

circ_element_descriptor_t mdel_demux =
  {
    .param_size = sizeof(demux_param_t),

    .symbol_name = "DEMUX",             /* symbol name used to identify this element */

    .pin_count = 13,                    /* alterable, the number of pins of device */

    .mdel_type = MDEL_DIGITAL,          /* the electrical type of this model */

    .mdel = &dsim_demux                 /* pointer to the model descriptor structure */
  };
