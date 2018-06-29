SUB_DEPTH = ./../..

include $(SUB_DEPTH)/autoconf.mk

CFLAGS += -Wno-unused-function -Wno-unused-but-set-variable

#########################################################################
# Targets.


OBJS += device.cxx.o \
		device-lib.cxx.o \
		

.PHONY: all clean dsimdevice

all: dsimdevice

clean: genclean

#########################################################################
# Standard rules.

include $(SUB_DEPTH)/build-aux/rules.generic.mk


#########################################################################
# Main rules.

dsimdevice: libdsimdevice.$(.LIB)

libdsimdevice.$(.LIB): $(OBJS)
