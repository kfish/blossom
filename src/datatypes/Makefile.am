## Process this file with automake to produce Makefile.in

AM_CFLAGS = -Wall -pedantic

INCLUDES = -I$(top_srcdir)/src/libjhash

noinst_HEADERS = \
	dictionary.h \
	x_tree.h

noinst_LIBRARIES = libdatatypes.a
libdatatypes_a_SOURCES = dictionary.c x_tree.c
libdatatypes_a_LIBADD = $(top_builddir)/src/libjhash/libjhash.a

#TESTS = dictionary-test

#noinst_PROGRAMS = $(TESTS)

#dictionary_test_SOURCES = dictionary-test.c
#dictionary_test_LDADD = \
#	$(top_builddir)/src/libjhash/libjhash.a \
#	libdatatypes.a

## Do not edit or modify anything in this comment block.
## The following line is a file identity tag for the GNU Arch
## revision control system.
##
## arch-tag: 4A0583C2-BF88-11D8-9891-000A95C7E2B6
