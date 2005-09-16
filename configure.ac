# Process this file with autoconf to produce a configure script.
AC_INIT
AC_CONFIG_SRCDIR([src/libblossom/blossom.c])

AC_PREREQ(2.53)

AC_CANONICAL_TARGET

AM_INIT_AUTOMAKE(blossom, 0.0.1)
AM_CONFIG_HEADER(config.h)

SHARED_VERSION_INFO="0:1:0"
SHLIB_VERSION_ARG=""

# Checks for programs.
AC_PROG_CC
AC_PROG_INSTALL
AC_PROG_MAKE_SET
AC_PROG_LIBTOOL

AC_C_CONST

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([fcntl.h stdlib.h string.h getopt.h unistd.h])

# Checks for doxygen
AC_CHECK_PROG(HAVE_DOXYGEN, doxygen, true, false)
AM_CONDITIONAL(HAVE_DOXYGEN,$HAVE_DOXYGEN)
if test $HAVE_DOXYGEN = "false"; then
             AC_MSG_WARN([*** doxygen not found, docs will not be built])
fi
  
# Check for docbook
AC_CHECK_PROG(HAVE_DOCBOOKTOMAN, docbook-to-man, true, false)
AM_CONDITIONAL(HAVE_DOCBOOKTOMAN,$HAVE_DOCBOOKTOMAN)
AC_CHECK_PROG(HAVE_DOCBOOK2HTML, docbook2html, true, false)
AM_CONDITIONAL(HAVE_DOCBOOK2HTML,$HAVE_DOCBOOK2HTML)

dnl Overall configuration success flag
blossom_config_ok=yes

dnl
dnl  Configuration option for building of experimental code.
dnl

ac_enable_experimental=no
AC_ARG_ENABLE(experimental,
     [  --enable-experimental   enable building of experimental code ],
     [ ac_enable_experimental=yes ])

if test "x${ac_enable_experimental}" = xyes ; then
    AC_DEFINE(ANX_CONFIG_EXPERIMENTAL, [], [Define to build experimental code])
fi

dnl
dnl Checks for getopt
dnl

# check for getopt in a separate library
HAVE_GETOPT=no
AC_CHECK_LIB(getopt, getopt, HAVE_GETOPT="yes")
if test "x$HAVE_GETOPT" = xyes ; then
  GETOPT_LIBS="-lgetopt"
  AC_SUBST(GETOPT_LIBS)
fi

# check for getopt_long in standard library
HAVE_GETOPT_LONG=no
AC_CHECK_FUNC(getopt_long, HAVE_GETOPT_LONG="yes")
if test "x$HAVE_GETOPT_LONG" = xyes ; then
  AC_DEFINE(HAVE_GETOPT_LONG, [], [Define to 1 if you have the 'getop_long' function])
else
  # FreeBSD has a gnugetopt library
  AC_CHECK_LIB(gnugetopt, getopt_long, HAVE_GETOPT_lONG="yes")
  if test "x$HAVE_GETOPT_LONG" = xyes ; then
    AC_DEFINE(HAVE_GETOPT_LONG, [],
      [Define to 1 if you have the 'getop_long' function])
    GETOPT_LONG_LIBS="-lgnugetopt"
    AC_SUBST(GETOPT_LONG_LIBS)
  fi
fi

dnl
dnl  Configuration tests complete -- provide summary of results.
dnl

if test $blossom_config_ok = no ; then

AC_MSG_RESULT([
**************************************************************

*** $PACKAGE $VERSION: Automatic configuration FAILED.

*** The file config.log has full details.

*** The following required libraries are missing or
*** misconfigured on your system:
])

AC_MSG_RESULT(
[*** If you install the required libraries from source, you
*** need to inform the dynamic linker of their location. If
*** you install them in a system-wide directory such as
*** /usr/local (the default), you must ensure that
*** /usr/local/lib is listed in /etc/ld.so.conf, then run
*** ldconfig to update the dynamic linking system.
*** Alternatively, you can set your LD_LIBRARY_PATH environment
*** variable to include the library installation directory.
])


AC_MSG_RESULT(
[**************************************************************
])


AC_MSG_ERROR([
***
*** After fixing the above problems, you must run ./configure again.
***
]) 

else

AC_SUBST(SHLIB_VERSION_ARG)
AC_SUBST(SHARED_VERSION_INFO)

AC_OUTPUT([
Makefile
doc/Makefile
doc/Doxyfile
include/Makefile
include/blossom/Makefile
src/Makefile
src/libblossom/Makefile
src/libblossom/Version_script
src/tools/Makefile
src/cgi-bin/Makefile
blossom.pc
])

AC_MSG_RESULT([
------------------------------------------------------------------------
  $PACKAGE $VERSION:  Automatic configuration OK.

  General configuration:

    Experimental code: ........... ${ac_enable_experimental}

  Tools:

    blossom

  Installation paths:

    Tools: ....................... ${prefix}/bin
    Documentation: ............... ${prefix}/share/doc/$PACKAGE

------------------------------------------------------------------------
])


fi
