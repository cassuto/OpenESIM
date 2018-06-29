## @file
# Config of openwsp
#

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

ifndef CONFIG_MK_
CONFIG_MK_ := 1

include $(SUB_DEPTH)/autoconf.mk

#########################################################################
# Tools

GEN-VERSION := $(SUB_DEPTH)/scripts/gen-version.py

#########################################################################
# Project dirs

OUT_DIR := out
OUT_TESTS_DIR = testcase

LIBOPENDSIM := $(SUB_DEPTH)/$(OUT_DIR)/libopendsim.$(.LIB)
LIBDSIMMODEL := $(SUB_DEPTH)/$(OUT_DIR)/libdsimmodel.$(.LIB)
LIBDSIMDEVICE := $(SUB_DEPTH)/$(OUT_DIR)/libdsimdevice.$(.LIB)

#########################################################################
# Toolchain

ifeq ($(CONFIG_BUILD_HOST),y)
  GCC       := $(HOST_GCC)
  GXX       := $(HOST_GXX)
  AR        := $(HOST_AR)
  RANLIB    := $(HOST_RANLIB)
else # BUILD TARGET
  GCC       := $(TARGET_GCC)
  GXX       := $(TARGET_GXX)
  AR        := $(TARGET_AR)
  RANLIB    := $(TARGET_RANLIB)
endif

#########################################################################
# Platform librarys

ifeq ($(CONFIG_TARGET_OS),win32)
  LIBS      += ws2_32
  LIBS      += winmm
  LIBS      += kernel32
endif

#########################################################################
# Globally Version Infomation Control

appVersionMajor = 0
appVersionMinor = 0
appVersionBuild = 1


#########################################################################
# Qt5 binaries dirs
#
ifneq ($(QT_PATH),)
  QT_INCLUDE := $(QT_PATH)/include
  QT_INCLUDE += $(QT_PATH)/include/QtCore
  QT_INCLUDE += $(QT_PATH)/include/QtGui
  QT_INCLUDE += $(QT_PATH)/include/QtWidgets
  QT_INCLUDE += $(QT_PATH)/include/QtPrintSupport
  QT_LIBS := $(QT_PATH)/lib
  QT_BIN := $(QT_PATH)/bin
  QT_UIC := $(QT_BIN)/uic
  QT_MOC := $(QT_BIN)/moc
  QT_RCC := $(QT_BIN)/rcc
else
  QT_UIC := uic
  QT_MOC := moc
  QT_RCC := rcc
endif

#########################################################################
# SDK - Qt5
#
ifneq ($(QT_PATH),)
  INCS += $(QT_INCLUDE)
  LIB_DIR += $(QT_LIBS)
endif
ifeq ($(USING_QT5),y)
  LIBS +=  Qt5Widgets Qt5Gui Qt5Concurrent Qt5Xml Qt5Core Qt5PrintSupport
  DEFS += UNICODE QT_NO_DEBUG QT_WIDGETS_LIB QT_GUI_LIB QT_CONCURRENT_LIB QT_XML_LIB QT_CORE_LIB  QT_PRINTSUPPORT_LIB
endif

endif # ifndef CONFIG_MK_
