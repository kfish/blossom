## Process this file with automake to produce Makefile.in

AM_CFLAGS = -Wall -pedantic

INCLUDES = -I$(top_srcdir)/src/libjhash -I$(top_srcdir)/src/datatypes

EXTRA_DIST = Version_script.in

noinst_HEADERS = \
	private.h \
	jhash.h \
	dictionary.h \
	xini.h \
	x_tree.h

# Libraries to build
lib_LTLIBRARIES = libblossom.la

libblossom_la_SOURCES = \
	blossom.c \
	config.c \
	datestamp.c \
	dictionary.c \
	flavour.c \
	input.c \
	jhash.c \
	path.c \
	template.c \
	xini.c \
	x_tree.c

libblossom_la_LDFLAGS = -version-info @SHARED_VERSION_INFO@ @SHLIB_VERSION_ARG@

TESTS = jhash-test dictionary-test

noinst_PROGRAMS = xini-test $(TESTS)

xini_test_SOURCES = xini-test.c
xini_test_LDADD = libblossom.la

jhash_test_SOURCES = jhash-test.c
jhash_test_LDADD = libblossom.la

dictionary_test_SOURCES = dictionary-test.c
dictionary_test_LDADD = libblossom.la
