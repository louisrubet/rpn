AUTOMAKE_OPTIONS = subdir-objects
ACLOCAL_AMFLAGS = ${ACLOCAL_FLAGS}


bin_PROGRAMS = rpn
rpn_SOURCES = src/object.cpp \
			src/program.cpp \
			src/main.cpp

rpn_CFLAGS =

rpn_LDFLAGS = -Wl,--no-as-needed -lreadline -lmpfr

dist_noinst_SCRIPTS = autogen.sh
