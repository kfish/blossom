## Process this file with automake to produce Makefile.in

AM_CFLAGS = -Wall -pedantic -g

INCLUDES = -I$(top_srcdir)/include

BLOSSOMDIR = ../libblossom
BLOSSOM_LIBS = $(BLOSSOMDIR)/libblossom.la

# Programs to build
bin_PROGRAMS = blossom.cgi

blossom_cgi_SOURCES = blossom.cgi.c
blossom_cgi_LDADD = $(BLOSSOM_LIBS)

## Do not edit or modify anything in this comment block.
## The following line is a file identity tag for the GNU Arch
## revision control system.
##
## arch-tag: 4A01387A-BF88-11D8-909B-000A95C7E2B6
