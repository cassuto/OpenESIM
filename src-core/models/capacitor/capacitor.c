/** @file
 * model-lib Capacitor Model
 */

/*
 * Equivalent model:
 *      ---1---             Req = dT/C
 *     |       |
 *     --      |            Ieq = C * Vn-1 / dT
 * Req ||     (x) Ieq
 *     --      |
 *     |       |
 *      ---2---
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

#include "capacitor.h"

asim_descriptor_t asim_capacitor =
  {
    .pfn_create = capacitor_create,     /* callback function to create parameters */

    .pfn_init = capacitor_init,         /* callback function to initialize the simulator model */

    .pfn_stamp = capacitor_stamp,       /* callback function to stamp node matrix */

    .pfn_vchanged = capacitor_vchanged, /* callback function to inform volt changed */

    .pfn_config = capacitor_config,     /* callback function to configure the element */

    .pfn_reset = NULL,                  /* callback function to reset the status */

    .pfn_uninit = capacitor_uninit      /* callback function to terminate the parameters */

  };

circ_element_descriptor_t mdel_capacitor =
  {
    .param_size = sizeof(capacitor_param_t),

    .symbol_name = "C",                 /* symbol name used to identify this element */

    .pin_count = 2,                     /* the number of pins of device */

    .mdel_type = MDEL_ANALOG,          /* the electrical type of this model */

    .mdel = &asim_capacitor             /* pointer to the model descriptor structure */
  };
