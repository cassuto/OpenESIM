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

lib_LTLIBRARIES = libdsimdevices.la

INCLUDES = -I$(top_builddir)/include      \
           -I$(top_srcdir)/include        \
           -I./include

#########################################################################
# Targets.

libdsimdevices_la_SOURCES = \
  1n4148/1n4148.cxx \
  diode/diode.cxx \
  register/register.cxx \
  capacitor/capacitor.cxx \
  inductor/inductor.cxx \
  vsource-dc/vsource-dc.cxx \
  vsource-dc-0v/vsource-dc-0v.cxx \
  voltprobe/voltprobe.cxx \
  atmega164/atmega164.cxx \
  lcdm5110/lcdm5110.cxx

libdsimdevices_la_SOURCES += \
  device.cxx \
  device-lib.cxx \
  propertyvalue.cxx

libdsimdevices_la_CFLAGS = -Wno-unused-function
libdsimdevices_la_CPPFLAGS = -Wno-unused-function
