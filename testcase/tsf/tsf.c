/**@file
 * Test framework
 */

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
#include <math.h>

#define TRACE_UNIT "tsf"

#include <dsim/logtrace.h>
#include <dsim/misc.h>
#include <dsim/string.h>

#include "test.h"


int
ds_test_init( const char *itemname )
{
  ds_log_init();

  trace_info(("name %s.\n", itemname));
  return 0;
}

int
ds_test_check2( int rc, const char *file, int line, const char *msg )
{
  if ( rc )
    {
      if ( msg )
        trace_error(("failed: %s:%d: rc = %d (%s)\n", file, line, rc, msg));
      else
        trace_error(("failed: %s:%d: rc = %d\n", file, line, rc));
      ds_test_exit( rc < 0 ? -rc : rc );
    }
  return rc;
}

int
ds_test_check_float2( float src, float dst, float error, const char *file, int line, const char *msg )
{
  if ( fabs( src - dst ) < error )
    return 0;
  else
    return ds_test_check2( 1, file, line, msg );
}

int
ds_test_exit( int rc )
{
  exit ( rc );
}
