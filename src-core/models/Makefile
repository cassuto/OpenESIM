SUB_DEPTH = ./../..

include $(SUB_DEPTH)/autoconf.mk

CFLAGS += -Wno-unused-function -Wno-unused-but-set-variable

#########################################################################
# Targets.

include register/makefile
include capacitor/makefile
include diode/makefile
include pn/makefile
include bjt/makefile
include mosfet/makefile
include opamp/makefile
include inductor/makefile
include vsource/makefile
include dac/makefile
include adc/makefile
include d-or/makefile
include d-xor/makefile
include d-and/makefile
include d-buff/makefile
include dev/makefile

OBJS += mdel-parameter.c.o \
		mdel-lib.c.o

.PHONY: all clean dsimmodel

all: dsimmodel

clean: genclean

#########################################################################
# Standard rules.

include $(SUB_DEPTH)/build-aux/rules.generic.mk
include $(SUB_DEPTH)/build-aux/rules.end.mk

#########################################################################
# Main rules.

dsimmodel: libdsimmodel.$(.LIB)

libdsimmodel.$(.LIB): $(OBJS)
