/**@file
 * Test framework
 */

/*
 *  OpenDSIM (Opensource Digital Circuit Simulation)
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

#define TRACE_UNIT "tsf"

#include <dsim/logtrace.h>
#include <dsim/misc.h>
#include <dsim/string.h>

#include "test.h"

////////////////////////////////////////////////////////////////////////////////

int
ds_test_init( const char *itemname )
{
  ds_log_init();

  trace_info(("name %s.\n", itemname));
  return 0;
}

int
ds_test_check( int rc, const char *msg )
{
  if ( rc )
    {
      if ( msg )
        trace_error(("failed: rc = %d (%s)\n", rc, msg));
      else
        trace_error(("failed: rc = %d\n", rc));
      ds_test_exit( rc < 0 ? -rc : rc );
    }
  return rc;
}

int
ds_test_exit( int rc )
{
  exit ( rc );
}
