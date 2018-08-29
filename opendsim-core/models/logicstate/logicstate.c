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

#include "logicstate.h"

dsim_descriptor_t dsim_logicstate =
  {
    .pfn_create = logicstate_create,    /* callback function to create parameters */

    .pfn_init = logicstate_init,        /* callback function to initialize the simulator model */

    .pfn_event = logicstate_event,      /* callback function to inform volt changed */

    .pfn_config = logicstate_config,    /* callback function to configure the element */

    .pfn_reset = NULL,                  /* callback function to reset the status */

    .pfn_uninit = logicstate_uninit     /* callback function to terminate the parameters */
  };

circ_element_descriptor_t mdel_logicstate =
  {
    .param_size = sizeof(logicstate_param_t),

    .symbol_name = "LOGICSTATE",        /* symbol name used to identify this element */

    .pin_count = 1,                     /* alterable, the number of pins of device */

    .mdel_type = MDEL_DIGITAL,          /* the electrical type of this model */

    .mdel = &dsim_logicstate            /* pointer to the model descriptor structure */
  };
