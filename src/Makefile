SUB_DEPTH = ./..

include $(SUB_DEPTH)/autoconf.mk

CFLAGS += -Wno-unused-function -Wno-unused-but-set-variable

#########################################################################
# Targets.

OBJS += util/logtrace.c.o \
		util/string.c.o \
		util/sys-memory.c.o \
		util/hashmap.c.o \
		util/list.c.o \
		util/rbtree.c.o \
		util/prime.c.o \
		util/bound.c.o \

OBJS += scheme-intr/lisp-buffer.c.o \
		scheme-intr/lisp-fault.c.o \
		scheme-intr/lisp-lexer.c.o \
		scheme-intr/lisp-parser.c.o \
		scheme-intr/lisp.c.o

OBJS += math/circ-matrix/circmatrix.c.o \

OBJS += circuit/circuit.c.o \
		circuit/circ-node.c.o \
		circuit/circ-element.c.o \

.PHONY: all clean subs subs_clean opendsim

all: subs opendsim

clean: subs_clean generic_clean

#########################################################################
# Standard rules.

include $(SUB_DEPTH)/targets/rules.generic.mk


#########################################################################
# Main rules.

opendsim: libopendsim.$(.LIB)

libopendsim.$(.LIB): $(OBJS)

subs:

subs_clean:
