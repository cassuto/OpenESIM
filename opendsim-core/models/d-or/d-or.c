/** @file
 * model-lib Digital OR gate Model
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

#include "mdel-lib-internal.h"

#undef D_GATE_NAME
#undef D_GATE_OPERATOR
#define D_GATE_NAME(name) or_ ## name
#define D_GATE_OPERATOR(inputs, sum) (inputs > 0)

#include "template-gate.h"

dsim_descriptor_t dsim_or =
  {
    .pfn_create = or_create,            /* callback function to create parameters */

    .pfn_init = or_init,                /* callback function to initialize the simulator model */

    .pfn_event = or_event,              /* callback function to inform volt changed */

    .pfn_config = or_config,            /* callback function to configure the element */

    .pfn_reset = NULL,                  /* callback function to reset the status */

    .pfn_uninit = or_uninit             /* callback function to terminate the parameters */
  };

circ_element_descriptor_t mdel_or =
  {
    .param_size = sizeof(d_gate_param_t),

    .symbol_name = "OR",                /* symbol name used to identify this element */

    .pin_count = 3,                     /* alterable, the number of pins of device */

    .mdel_type = MDEL_DIGITAL,          /* the electrical type of this model */

    .mdel = &dsim_or                    /* pointer to the model descriptor structure */
  };
