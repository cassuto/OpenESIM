/*
 *  OpenDSIM (Opensource Circuit Simulator)
 *  Copyright (C) 2016, The first Middle School in Yongsheng Lijiang China
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

#ifndef TEST_H_
#define TEST_H_

#include <dsim/misc.h>

#define ds_test_check(rc,msg)                       ds_test_check2(rc, DS_CURRENT_FILE, DS_CURRENT_LINE, msg)
#define ds_test_check_float(src,dst,error, msg)     ds_test_check_float2(src,dst,error, DS_CURRENT_FILE, DS_CURRENT_LINE, msg)

int ds_test_init( const char *itemname );
int ds_test_check2( int rc, const char *file, int line, const char *msg );
int ds_test_exit( int rc );
int ds_test_check_float2( float src, float dst, float error, const char *file, int line, const char *msg );

#endif //!defined(TEST_H_)
