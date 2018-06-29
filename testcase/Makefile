.PHONY: all clean makelib dotest

SUB_DEPTH = ./..

include $(SUB_DEPTH)/autoconf.mk

CFLAGS += -Wno-unused-function -Wno-unused-but-set-variable

#########################################################################
# Targets.

LIBS += opendsim dsimmodel dsimdevice
OBJS += tsf/tsf.c.o
INCS += tsf

CONFIG_TESTCASE := y

SRC := test-hashmap.c
SRC += test-list.c
SRC += test-rbtree.c
SRC += test-bound.c
SRC += test-circmatrix.c
SRC += test-circ-element-create.c
SRC += test-circuit.c
SRC += test-mdel-register.c
SRC += test-mdel-diode.c
SRC += test-mdel-adc.c
SRC += test-mdel-dac.c
SRC += test-mdel-or.c
SRC += test-scheme-lex.c

SRCS_TEST = $(SRC)

#########################################################################
# Building rules.

include $(SUB_DEPTH)/build-aux/rules.generic.mk

all: makelib dotest

clean: genclean


EXECS_TEST := $(foreach name,$(SRCS_TEST),$(patsubst %.c,%.$(.EXEC),$(name)))


define GENERATE_LINK_RULE
$(1): $(OBJS) $(LIBOPENDSIM) $(2)
endef

$(foreach exec,$(EXECS_TEST),$(eval $(call GENERATE_LINK_RULE,$(exec),$(patsubst %.$(.EXEC),%.c.o,$(exec)))) )

POST_CMD := $(foreach exec,$(EXECS_TEST),./$(exec) && )

makelib:
	@$(MAKE) -C $(SUB_DEPTH)/src -f Makefile.lib all

dotest: $(EXECS_TEST)
	@$(POST_CMD) echo.
	@echo =====================================
	@echo test: all tests have succeeded!
	@echo =====================================
