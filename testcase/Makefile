SUB_DEPTH = ./..

include $(SUB_DEPTH)/autoconf.mk

CFLAGS += -Wno-unused-function -Wno-unused-but-set-variable

#########################################################################
# Targets.

SRC := $(SUB_DEPTH)/src
OBJS += $(SRC)/util/logtrace.c.o \
		$(SRC)/util/string.c.o \
		$(SRC)/util/sys-memory.c.o \
		$(SRC)/util/hashmap.c.o \
		$(SRC)/util/list.c.o \

OBJS += $(SRC)/scheme-intr/lisp-buffer.c.o \
		$(SRC)/scheme-intr/lisp-fault.c.o \
		$(SRC)/scheme-intr/lisp-lexer.c.o \
		$(SRC)/scheme-intr/lisp-parser.c.o \
		$(SRC)/scheme-intr/lisp.c.o

OBJS += tsf/tsf.c.o
        
INCS += tsf
INCS += $(SRC)

CONFIG_TESTCASE := y

OBJS_TEST_SCHEME_LEX = test-scheme-lex.c.o
OBJS_TEST_HASHMAP = test-hashmap.c.o

.PHONY: all clean dotest

all: dotest

clean: generic_clean

#########################################################################
# Standard rules.

include $(SUB_DEPTH)/targets/rules.generic.mk


#########################################################################
# Main rules.


test-scheme-lex.$(.EXEC): $(OBJS) $(OBJS_TEST_SCHEME_LEX)
test-hashmap.$(.EXEC): $(OBJS) $(OBJS_TEST_HASHMAP)

dotest: test-scheme-lex.$(.EXEC) test-hashmap.$(.EXEC)
	@./test-hashmap
	@./test-scheme-lex
	@echo =====================================
	@echo test: all tests have succeeded!
	@echo =====================================
