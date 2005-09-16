## Process this file with automake to produce Makefile.in

AM_CFLAGS = -Wall -pedantic -g

INCLUDES = -I$(top_srcdir)/include

BLOSSOMDIR = ../libblossom
BLOSSOM_LIBS = $(BLOSSOMDIR)/libblossom.la

# Programs to build
bin_PROGRAMS = blossom

blossom_SOURCES = blossom-tool.c
blossom_LDADD = $(BLOSSOM_LIBS)

## Do not edit or modify anything in this comment block.
## The following line is a file identity tag for the GNU Arch
## revision control system.
##
## arch-tag: 4A18487F-BF88-11D8-9113-000A95C7E2B6
