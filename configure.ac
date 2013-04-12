AC_PREREQ([2.67])

m4_include([m4/version.m4])
m4_include([m4/lib_version.m4])

AC_INIT([rgxg], [_PACKAGE_VERSION], [rgxg-devel@lists.sourceforge.net], [rgxg], [rgxg.sf.net])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR(build-aux)
AC_CONFIG_SRCDIR([src/main.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile lib/rgxg.pc])

LIB_CURRENT=_LIB_CURRENT
LIB_REVISION=_LIB_REVISION
LIB_AGE=_LIB_AGE
AC_SUBST(LIB_CURRENT)
AC_SUBST(LIB_REVISION)
AC_SUBST(LIB_AGE)

AM_INIT_AUTOMAKE([1.10 -Wall -Werror silent-rules subdir-objects])

AC_PROG_CC_C99
AC_PROG_INSTALL
AM_PROG_CC_C_O
m4_ifdef([AM_PROG_AR], [AM_PROG_AR])
AC_PROG_LIBTOOL
PKG_PROG_PKG_CONFIG

AC_CHECK_HEADERS([limits.h stddef.h])

AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT8_T

AC_FUNC_MALLOC
AC_CHECK_FUNCS([regcomp strtol])

LT_PREREQ([2.2])
LT_INIT([dlopen])

AC_OUTPUT
