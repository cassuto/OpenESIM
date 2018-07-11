/** @file
 * model-lib Device Model Base
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

#include "dev.h"

asim_descriptor_t asim_dev =
  {
    .pfn_create = adev_create,           /* callback function to create parameters */

    .pfn_init = adev_init,               /* callback function to initialize the simulator model */

    .pfn_stamp = adev_stamp,             /* callback function to stamp node matrix */

    .pfn_vchanged = adev_vchanged,       /* callback function to inform volt changed */

    .pfn_config = adev_config,           /* callback function to configure the element */

    .pfn_reset = adev_reset,             /* callback function to reset the status */

    .pfn_uninit = adev_uninit            /* callback function to terminate the parameters */
  };
  
dsim_descriptor_t dsim_dev =
  {
    .pfn_create = ddev_create,           /* callback function to create parameters */

    .pfn_init = ddev_init,               /* callback function to initialize the simulator model */

    .pfn_stamp = ddev_stamp,             /* callback function to stamp node matrix */

    .pfn_event = ddev_event,             /* callback function to inform volt changed */

    .pfn_config = ddev_config,           /* callback function to configure the element */

    .pfn_reset = ddev_reset,             /* callback function to reset the status */

    .pfn_uninit = ddev_uninit            /* callback function to terminate the parameters */
  };

circ_element_descriptor_t mdel_adev =
  {
    .param_size = sizeof(adev_param_t),

    .symbol_name = "_ADEV_",            /* symbol name used to identify this element */

    .pin_count = 2,                     /* alterable, the number of pins of device */

    .mdel_type = MDEL_ANALOG,           /* the electrical type of this model */

    .mdel = &asim_dev                   /* pointer to the model descriptor structure */
  };
  

circ_element_descriptor_t mdel_ddev =
  {
    .param_size = sizeof(ddev_param_t),

    .symbol_name = "_DDEV_",            /* symbol name used to identify this element */

    .pin_count = 2,                     /* alterable, the number of pins of device */

    .mdel_type = MDEL_DIGITAL,          /* the electrical type of this model */

    .mdel = &dsim_dev                   /* pointer to the model descriptor structure */
  };
