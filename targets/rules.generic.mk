## @file
# Rules - generic
#

#
#  OpenDSIM (Opensource Digital Circuit Simulation)
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

include $(SUB_DEPTH)/config.mk
include $(SUB_DEPTH)/targets/rules.mk


#########################################################################
# Define the generic rules

CC := $(GCC)
CXX := $(GXX)

CFLAGS += -std=c99 -O1 -Wall -Wstrict-prototypes -Wno-unused-variable
CXXFLAGS += -O1 -Wall -Wno-unused-variable
LDFLAGS += -lstdc++
ARFLAGS +=

ifeq ($(CONFIG_TARGET_OS),win32)
LDFLAGS += -Wl,-subsystem,console
endif

ifeq ($(CONFIG_MAK_TYPE),debug)
CFLAGS += -g
CXXFLAGS += -g
LDFLAGS += -g
endif

CFLAGS   += $(addprefix -I, $(CINCS) $(CXXINCS) $(INCS)) $(addprefix -D, $(DEFS))
CXXFLAGS += $(addprefix -I, $(CINCS) $(CXXINCS) $(INCS)) $(addprefix -D, $(DEFS))
LDFLAGS  += $(addprefix -L, $(LIB_DIR)) $(addprefix -l, $(LIBS))


#########################################################################
# building files

%.s.o: %.s
	$(call run-command,$(CC) $(CFLAGS) -c $< -o $@,"  CC        $@")

%.s.d: %.s
	$(call run-command,$(CC) $(CFLAGS) -MM -MF $@ $<,"  DEP       $@")
	-@sed -i 's,\($(notdir $*)\)\.o[ :]*,$<.o: ,g' '$@'

%.c.o: %.c
	$(call run-command,$(CC) $(CFLAGS) -c $< -o $@,"  CC        $@")

%.c.d: %.c
	$(call run-command,$(CC) $(CFLAGS) -MM -MF $@ $<,"  DEP       $@")
	-@sed -i 's,\($(notdir $*)\)\.o[ :]*,$<.o: ,g' '$@'

%.cpp.o: %.cpp
	$(call run-command,$(CXX) $(CXXFLAGS) -c $< -o $@,"  CXX       $@")

%.cpp.d: %.cpp
	$(call run-command,$(CXX) $(CXXFLAGS) -MM -MF $@ $<,"  DEP      $@")
	-@sed -i 's,\($(notdir $*)\)\.o[ :]*,$<.o: ,g' '$@'

%.$(.EXEC):
	$(call run-command,$(CC) $(filter %.o %.a,$^) $(LDFLAGS) -o $@,"  LINK      $@")
ifneq ($(CONFIG_TESTCASE),y)
	@$(MV) $@ $(SUB_DEPTH)/$(OUT_DIR)/$@
endif

%.$(.LIB):
	$(call run-command,$(AR) $(ARFLAGS) $@ $(filter %.o,$^) > /dev/null,"  AR        $@")
	$(RANLIB) $@
	@$(MV) $@ $(SUB_DEPTH)/$(OUT_DIR)/$@
