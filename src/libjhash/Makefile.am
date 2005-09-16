## Process this file with automake to produce Makefile.in

AM_CFLAGS = -Wall -pedantic

noinst_LIBRARIES = libjhash.a
libjhash_a_SOURCES = jhash.c

noinst_HEADERS = \
	jhash.h

TESTS = jhash-test

noinst_PROGRAMS = $(TESTS)

jhash_test_SOURCES = jhash-test.c
jhash_test_LDADD = libjhash.a

## Do not edit or modify anything in this comment block.
## The following line is a file identity tag for the GNU Arch
## revision control system.
##
## arch-tag: 4A0E5121-BF88-11D8-9C6D-000A95C7E2B6
