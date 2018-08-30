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

#include "d-lcd8544.h"

dsim_descriptor_t dsim_lcd8544 =
  {
    .pfn_create = lcd8544_create,       /* callback function to create parameters */

    .pfn_init = lcd8544_init,           /* callback function to initialize the simulator model */

    .pfn_config = lcd8544_config,       /* callback function to configure the element */

    .pfn_reset = lcd8544_reset,         /* callback function to reset the status */

    .pfn_uninit = lcd8544_uninit,       /* callback function to terminate the parameters */

    .pfn_clock = NULL,                  /* callback function to inform clock signal triggered */

    .pfn_event = lcd8544_event,         /* callback function to inform volt changed */
  };

circ_element_descriptor_t mdel_lcd8544 =
  {
    .param_size = sizeof(lcd8544_param_t),

    .symbol_name = "LCD8544",           /* symbol name used to identify this element */

    .pin_count = 5,                     /* alterable, the number of pins of device */

    .mdel_type = MDEL_DIGITAL,          /* the electrical type of this model */

    .mdel = &dsim_lcd8544               /* pointer to the model descriptor structure */
  };
