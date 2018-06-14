SUB_DEPTH = ./..

include $(SUB_DEPTH)/autoconf.mk

CFLAGS += -Wno-unused-function -Wno-unused-but-set-variable

#########################################################################
# Targets.

LIBS += opendsim
OBJS += tsf/tsf.c.o

INCS += tsf
INCS += $(SRC)

CONFIG_TESTCASE := y

SRCS_TEST := \
	test-hashmap.c \
	test-list.c \
	test-rbtree.c \
	test-bound.c \
	test-circmatrix.c \
	test-scheme-lex.c

.PHONY: all clean makelib dotest

all: makelib dotest

clean: generic_clean

#########################################################################
# Standard rules.

include $(SUB_DEPTH)/targets/rules.generic.mk


#########################################################################
# Main rules.

EXECS_TEST := $(foreach name,$(SRCS_TEST),$(patsubst %.c,%.$(.EXEC),$(name)))


define GENERATE_LINK_RULE
$(1): $(OBJS) $(LIBOPENDSIM) $(2)
endef

$(foreach exec,$(EXECS_TEST),$(eval $(call GENERATE_LINK_RULE,$(exec),$(patsubst %.$(.EXEC),%.o,$(exec)))) )
POST_CMD := $(foreach exec,$(EXECS_TEST),./$(exec) && )

makelib:
	@make -C $(SUB_DEPTH)/src all

dotest: $(EXECS_TEST)
	@$(POST_CMD) echo.
	@echo =====================================
	@echo test: all tests have succeeded!
	@echo =====================================
