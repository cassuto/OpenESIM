SUB_DEPTH = ./../..

include $(SUB_DEPTH)/autoconf.mk

CFLAGS += -Wno-unused-function -Wno-unused-but-set-variable
CXXFLAGS += -Wno-unused-function

#########################################################################
# Targets.

OBJS += 1n4148/1n4148.cxx.o \
		diode/diode.cxx.o \
		res/res.cxx.o \
		
OBJS += device.cxx.o \
		device-lib.cxx.o
		
.PHONY: all clean dsimdevice

all: dsimdevice

clean: genclean

#########################################################################
# Standard rules.

include $(SUB_DEPTH)/build-aux/rules.generic.mk
include $(SUB_DEPTH)/build-aux/rules.end.mk

#########################################################################
# Main rules.

dsimdevice: libdsimdevice.$(.LIB)

libdsimdevice.$(.LIB): $(OBJS)
