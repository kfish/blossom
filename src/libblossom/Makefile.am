## Process this file with automake to produce Makefile.in

AM_CFLAGS = -Wall -pedantic

INCLUDES = -I$(top_srcdir)/src/libjhash -I$(top_srcdir)/src/datatypes

EXTRA_DIST = Version_script.in

noinst_HEADERS = \
	private.h \
	xini.h

# Libraries to build
lib_LTLIBRARIES = libblossom.la

libblossom_la_SOURCES = \
	blossom.c \
	config.c \
	datestamp.c \
	flavour.c \
	input.c \
	path.c \
	template.c \
	xini.c

libblossom_la_LDFLAGS = -version-info @SHARED_VERSION_INFO@ @SHLIB_VERSION_ARG@
libblossom_la_LIBADD = \
	$(top_builddir)/src/libjhash/libjhash.a \
	$(top_builddir)/src/datatypes/libdatatypes.a

##TESTS = xini-test

##noinst_PROGRAMS = $(TESTS)

##xini_test_SOURCES = xini.c xini.h xini-test.c
## Do not edit or modify anything in this comment block.
## The following line is a file identity tag for the GNU Arch
## revision control system.
##
## arch-tag: 4A0A1468-BF88-11D8-88D7-000A95C7E2B6
