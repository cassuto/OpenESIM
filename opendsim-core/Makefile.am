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

INCLUDES = -I$(top_builddir)/include      \
           -I$(top_srcdir)/include        \
           -I$(top_builddir)/opendsim-core  \
           -I$(top_builddir)/opendsim-frontend

lib_LTLIBRARIES = libdsim.la

libdsim_la_SOURCES =  util/logtrace.c \
                util/string.c \
                util/sys-memory.c \
                util/sys-memory-cpp.cxx \
                util/hashmap.c \
                util/list.c \
                util/rbtree.c \
                util/prime.c \
                util/bound.c \
                util/errors.c \
                scheme-intr/lisp-buffer.c \
                scheme-intr/lisp-fault.c \
                scheme-intr/lisp-lexer.c \
                scheme-intr/lisp-parser.c \
                scheme-intr/lispex-units.c \
                scheme-intr/lisp.c \
                math/circ-matrix/circmatrix.c \
                circuit/circuit.c \
                circuit/circ-pin.c \
                circuit/circ-node.c \
                circuit/circ-element.c
        
libdsim_la_CFLAGS = -Wno-unused-function
libdsim_la_CPPFLAGS = -Wno-unused-function

BUILT_SOURCES = errors-generated.h version-generated.h
CLEANFILES = errors-generated.h version-generated.h

SUBDIRS = models

#
# Generate the status code descriptors.
#
errors-generated.h: \
		util/errors.sed \
		$(top_builddir)/include/dsim/error.h \
        $(top_builddir)/include/frontend/error.h  
	$(SED) -f $< $(filter %.h,$^) > $@

#
# Generate version infomation
#
appName = OpenDSIM (A/D mixed circuit simulator)
appNameShort = OpenDSIM

version-generated.h:
	$(PYTHON) "$(top_builddir)/scripts/gen-version.py" \
       "$(appName)" "$(appNameShort)" \
       "$(appVersionMajor)" "$(appVersionMinor)" "$(appVersionBuild)" \
       version-generated.h
