ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = librgxg.la
librgxg_la_CFLAGS = -I$(top_srcdir)/include  -I$(top_srcdir)/lib
librgxg_la_SOURCES = lib/common_macros.h lib/internal_utils.h \
					 lib/internal_utils.c lib/number.c lib/utils.c lib/net.c
librgxg_la_LDFLAGS = -no-undefined \
					 -version-info $(LIB_CURRENT):$(LIB_REVISION):$(LIB_AGE)

librgxgincludedir = $(includedir)/rgxg/
librgxginclude_HEADERS = include/rgxg/number.h include/rgxg/types.h \
						 include/rgxg/utils.h include/rgxg/net.h

bin_PROGRAMS = rgxg
rgxg_SOURCES = src/main.c src/rgxg.h \
			   src/common_macros.h \
			   src/alternation.h src/alternation.c \
			   src/cidr.h src/cidr.c \
			   src/escape.h src/escape.c \
			   src/range.h src/range.c
rgxg_CFLAGS = -I$(top_srcdir)/include
rgxg_LDADD = librgxg.la

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = lib/rgxg.pc

man_MANS = man/rgxg.1

EXTRA_DIST = lib/rgxg.pc.in $(man_MANS)
dist_doc_DATA=README

DISTCLEANFILES = lib/rgxg.pc
