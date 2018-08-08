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

#include <dsim/memory.h>
#include <device/device.h>

ModelPropertyValue::ModelPropertyValue()
                   : type( MDEL_VAR_UNKNOWN )
                   , index( 0 )
                   , next( 0l ) { u.vsp = 0l; }

ModelPropertyValue::~ModelPropertyValue() { if( type == MDEL_VAR_STRPTR ) ds_heap_free( u.vsp ); }

DevicePropertyValue::DevicePropertyValue()
                   : type( DEV_VAR_UNKNOWN )
                   , index( 0 )
                   , next( 0l ) { u.vsp = 0l; }

DevicePropertyValue::~DevicePropertyValue() { if( type == DEV_VAR_STRPTR ) ds_heap_free( u.vsp ); }
