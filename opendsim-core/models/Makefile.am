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

lib_LTLIBRARIES = libdsimmodels.la

INCLUDES = -I$(top_builddir)/include      \
           -I$(top_srcdir)/include        \
           -I./include

#########################################################################
# Targets.

libdsimmodels_la_SOURCES =
libdsimmodels_la_LIBADD =

include register/makefile.inc
include capacitor/makefile.inc
include diode/makefile.inc
include pn/makefile.inc
include bjt/makefile.inc
include mosfet/makefile.inc
include opamp/makefile.inc
include inductor/makefile.inc
include vsource/makefile.inc
include dac/makefile.inc
include adc/makefile.inc
include d-or/makefile.inc
include d-xor/makefile.inc
include d-and/makefile.inc
include d-buff/makefile.inc
include d-mcu-avr/makefile.inc
include d-lcd8544/makefile.inc
include d-mux/makefile.inc
include d-demux/makefile.inc
include d-drvsrc/makefile.inc
include logicstate/makefile.inc
include dev/makefile.inc

libdsimmodels_la_SOURCES += mdel-parameter.c \
                      mdel-lib.c

libdsimmodels_la_CFLAGS = -Wno-unused-function
libdsimmodels_la_CPPFLAGS = -Wno-unused-function
