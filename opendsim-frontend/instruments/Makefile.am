#
#  OpenDSIM (A/D mixed circuit simulator)
#  Copyleft (C) 2016, The first Middle School in Yongsheng Lijiang China
#
#  This project is free software; you can redistribute it and/or
#  modify it under the terms of the GNU Lesser General Public License(GPL)
#  as published by the Free Software Foundation; either version 2.1
#  of the License, or (at your option) any later version.
#
#  This project is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
#  Lesser General Public License for more details.
#

lib_LTLIBRARIES = libdsiminstruments.la

INCLUDES = -I$(top_builddir)/include      \
           -I$(top_srcdir)/include        \
           -I./include                    \
           $(QT_INCLUDES)

#########################################################################
# Targets.

libdsiminstruments_la_SOURCES = \
  oscilloscope/oscilloscope.cxx \
  oscilloscope/oscopeform.cxx \
  oscilloscope/oscoperenderer.cxx \
  plotter/plotter.cxx \
  plotter/plotterform.cxx \
  plotter/plotterrenderer.cxx

libdsiminstruments_la_SOURCES += \
  instrumentbase.cxx \
  instrument-lib.cxx \
  instrumentpanel.cxx
  
MOCHEADERS = \
  include/inst_oscilloscope.h \
  include/inst_plotter.h \
  oscilloscope/oscopeform.h \
  plotter/plotterform.h

MOCFILES = $(MOCHEADERS:.h=.moc)

*/%.moc: */%.h
	$(MOC) -o $@ $<
  
libdsiminstruments_la_CFLAGS = -Wno-unused-function
libdsiminstruments_la_CPPFLAGS = -Wno-unused-function

BUILT_SOURCES = $(MOCFILES)
CLEANFILES = $(MOCFILES)
