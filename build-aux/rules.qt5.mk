## @file
# Rules - QT5 specifical
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

#########################################################################
# Include generic part

USING_QT5 = y

include $(SUB_DEPTH)/config.mk
include $(SUB_DEPTH)/build-aux/rules.mk

#########################################################################
# Scan Qt source files

define GENERATE_SIN_DEP_RULE
$(1): $(2)
endef

$(foreach src,$(QT_SRCS),$(eval $(call GENERATE_SIN_DEP_RULE,$(addsuffix .o,$(src)),$(patsubst %.cxx,%.moc.cxx,$(src)))) )

OBJS += $(foreach src,$(QT_SRCS),$(addsuffix .o,$(src)))
OBJS += $(foreach src,$(QT_SRCS),$(patsubst %.cxx,%.moc.cxx.o,$(src)))

OBJS += $(foreach src,$(QT_RCCS),$(patsubst %.qrc,%.rcc.cxx.o,$(src)))

FILTEROUT_OBJDEP += %.moc.cxx.o %.rcc.cxx.o

#########################################################################
# building files

%.moc.cxx: %.cxx
%.rcc.cxx: %.rcc

%.moc.cxx.o: %.moc.cxx
%.rcc.cxx.o: %.rcc.cxx

%.h: %.ui
	$(call run-command,$(QT_UIC) $< -o $@,"  UIC      $<")

%.moc.cxx: %.h
	$(call run-command,$(QT_MOC) $< -o $@,"  MOC      $@")
    
%.rcc.cxx: %.qrc
	$(call run-command,$(QT_RCC) -name $(basename $(notdir $<)) $< -o $@,"  RCC      $@")
