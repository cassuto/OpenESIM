/* ****************************************************************************
 * OpenESIM (Electrical Engineering Simulator)                                *
 * Copyright (C) 2019, ESIM team. <diyer175@hotmail.com>                      *
 *                                                                            *
 * This project is free software; you can redistribute it and/or              *
 * modify it under the terms of the GNU Lesser General Public License(GPL)    *
 * as published by the Free Software Foundation; either version 2.1           *
 * of the License, or (at your option) any later version.                     *
 *                                                                            *
 * This project is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU          *
 * Lesser General Public License for more details.                            *
 * ****************************************************************************/

#include "mainwindow.h"
#include <QApplication>

static void messageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
  QByteArray localMsg = msg.toLocal8Bit();
  const char *file = context.file ? context.file : "";
  const char *function = context.function ? context.function : "";
  switch (type)
  {
  case QtDebugMsg:
    fprintf(stderr, "Debug: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
    break;
  case QtInfoMsg:
    fprintf(stderr, "Info: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
    break;
  case QtWarningMsg:
    fprintf(stderr, "Warning: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
    break;
  case QtCriticalMsg:
    fprintf(stderr, "Critical: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
    break;
  case QtFatalMsg:
    fprintf(stderr, "Fatal: %s (%s:%u, %s)\n", localMsg.constData(), file, context.line, function);
#if 1
    __asm__ __volatile__("int $3\n\tnop\n\t"); // Breakpoint on x86
#else
    abort();
#endif
    break;
  }
}

int main(int argc, char *argv[])
{
  qInstallMessageHandler(messageOutput);

  QCoreApplication::setOrganizationName("OpenESIM");
  QCoreApplication::setOrganizationDomain("openesim.com");
  QCoreApplication::setApplicationName("OpenESIM");

  QApplication app(argc, argv);
  esim::MainWindow *w = new esim::MainWindow();
  w->show();
  return app.exec();
}
