## Process this file with automake to produce Makefile.in

AM_CFLAGS = -Wall -pedantic -g

INCLUDES = -I$(top_srcdir)/include

BLOSSOMDIR = ../libblossom
BLOSSOM_LIBS = $(BLOSSOMDIR)/libblossom.la

# Programs to build
bin_PROGRAMS = blossom

blossom_SOURCES = blossom-tool.c
blossom_LDADD = $(BLOSSOM_LIBS)
