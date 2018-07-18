/** @file
 * OpenDSIM - Editor Program Main Entry
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
#define TRACE_UNIT "main"

#include <dsim/logtrace.h>
#include <dsim/device-lib.h>
#include <dsim/error.h>
#include <dsim/misc.h>
#include <dsim/version.h>

#include <new>
#include <QApplication>
#include <QStyleFactory>

#include "mainwindow.h"

static char *reserved = 0l;
static bool noMemory = false;

using namespace dsim;

static void new_handler()
{
  if( !noMemory )
    {
      noMemory = true;
      delete [] reserved;
      trace_error(("no memory"));

      MainWindow::instance()->noMemory();
    }
  exit( 1 );
}

int main( int argc, char *argv[] )
{
  int rc = 0;

  ds_log_init();
  trace_info(("%s\n", appBanner));

  QApplication app( argc, argv );
  QApplication::setOrganizationName( appNameShort );
  QApplication::setApplicationName( appNameShort );

  reserved = new char[64 * 1024 * 1024];
  std::set_new_handler( new_handler );

  trace_info(("loading device library...\n"));

  if( device_lib_init() )
    {
      trace_error(("failed to initialize device library!\n"));
      return 1;
    }

  trace_info(("loading main window...\n"));

  MainWindow mainwnd;

  mainwnd.show();

  rc = app.exec();
  trace_info(("terminated. rc = %d\n", rc));
  return rc;
}
