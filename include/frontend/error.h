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

#ifndef FRONTEND_ERROR_H_
#define FRONTEND_ERROR_H_

#include <dsim/error.h>

/*
 * The following Macro definitions participate in automatic generation of error
 * messages data ( errors-generated.h ), which begins with gen\{\{ and ends with \}\}gen
 * comment.
 */

/* gen{{ */

/** Target element ID is invalid */
#define DS_INVALID_ELEMENT_ID (DS_FRONTEND+1)
/** Target element symbol is invalid */
#define DS_INVALID_ELEMENT_SYMBOL (DS_FRONTEND+2)
/** Target device symbol is invalid */
#define DS_INVALID_DEVICE_SYMBOL (DS_FRONTEND+3)
/** Can't resolve sub elements */
#define DS_SOLVE_SUBELEMENTS (DS_FRONTEND+4)
/** Target element has been referenced before */
#define DS_REREFERENCE (DS_FRONTEND+5)
/** Failed to create device instance */
#define DS_CREATE_IDEVICE (DS_FRONTEND+6)
/** Target custom template is invalid */
#define DS_INVALID_CUSTOM_TEMPLATE (DS_FRONTEND+7)
/** Failed to create circuit matrix */
#define DS_CREATE_CIRCUIT_MATRIX (DS_FRONTEND+8)
/** Failed to create circuit simulator */
#define DS_CREATE_CIRCUIT_SIMULATOR (DS_FRONTEND+9)
/** Failed to create circuit node object */
#define DS_CREATE_CIRCUIT_NODE (DS_FRONTEND+10)
/** Target pin reference is invalid */
#define DS_INVALID_PIN_REFERENCE (DS_FRONTEND+11)
/** Invalid Type of property */
#define DS_INVALID_PROPERTY_TYPE (DS_FRONTEND+12)
/** Unknown property entry */
#define DS_UNKNOWN_PROPERTY_ENTRY (DS_FRONTEND+13)
/** Can't find the device */
#define DS_FIND_DEVICE (DS_FRONTEND+14)

/* }}gen */

#endif //!defined(FRONTEND_ERROR_H_)
