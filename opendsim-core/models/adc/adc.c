/** @file
 * model-lib ADC (Analog to Digital Converter) Bridge
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

#include "adc.h"

adsim_descriptor_t adsim_adc =
  {
    .pfn_create = adc_create,           /* callback function to create parameters */

    .pfn_init = adc_init,               /* callback function to initialize the simulator model */

    .pfn_stamp = adc_stamp,             /* callback function to stamp node matrix */

    .pfn_vchanged = adc_vchanged,       /* callback function to inform volt changed */

    .pfn_config = adc_config,           /* callback function to configure the element */

    .pfn_reset = NULL,                  /* callback function to reset the status */

    .pfn_uninit = adc_uninit,           /* callback function to terminate the parameters */

    .pfn_clock = NULL,                  /* callback function to inform clock signal triggered */

    .pfn_event = NULL                   /* callback function to inform volt changed */
  };

circ_element_descriptor_t mdel_adc =
  {
    .param_size = sizeof(adc_param_t),

    .symbol_name = "ADC",               /* symbol name used to identify this element */

    .pin_count = 2,                     /* alterable, the number of pins of device */

    .mdel_type = MDEL_AD,               /* the electrical type of this model */

    .mdel = &adsim_adc                  /* pointer to the model descriptor structure */
  };
