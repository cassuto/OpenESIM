/*
 *  OpenDSIM (A/D mixed circuit simulator)
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

#include <stdlib.h>
#include <stdio.h>

#define TRACE_UNIT "tsf"

#include <dsim/logtrace.h>
#include <dsim/string.h>
#include <dsim/memory.h>

#include <dsim/bound.h>

#include "tsf/test.h"



int
main( int argc, char *argv[] )
{
  static unsigned long seq[] = { 4, 8, 10, 12, 14, 16, 18, 20 };
  if ( ds_test_init( "test-bound" ) ) return 1;

  const unsigned long * p = lower_bound( seq+2, seq+6, 12 );
  ds_test_check( (*p != 12 ), "lower_bound()" );

  p = lower_bound( seq+2, seq+6, 11 );
  ds_test_check( (*p != 12 ), "lower_bound()" );

  p = upper_bound( seq+2, seq+6, 16 );
  ds_test_check( (*p != 18 ), "upper_bound()" );

  p = upper_bound( seq+2, seq+6, 15 );
  ds_test_check( (*p != 16 ), "upper_bound()" );

  return 0;
}
