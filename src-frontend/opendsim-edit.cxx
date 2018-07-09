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
#define TRACE_UNIT "init"

#include <dsim/logtrace.h>
#include <dsim/device-lib.h>
#include <dsim/error.h>
#include <dsim/misc.h>

#include <new>
#include <QApplication>

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
  QApplication app( argc, argv );

  ds_log_init();

  reserved = new char[64 * 1024 * 1024];
  std::set_new_handler( new_handler );

  if( device_lib_init() )
    {
      trace_error(("failed to initialize device library!\n"));
      return 1;
    }

  MainWindow mainwnd;

  mainwnd.show();

  return app.exec();
}
