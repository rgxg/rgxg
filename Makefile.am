ACLOCAL_AMFLAGS = -I m4

lib_LTLIBRARIES = librgxg.la
librgxg_la_CFLAGS = -I$(top_srcdir)/include  -I$(top_srcdir)/lib
librgxg_la_SOURCES = lib/common_macros.h lib/internal_utils.h \
					 lib/internal_utils.c lib/number.c lib/utils.c lib/net.c
librgxg_la_LDFLAGS = -no-undefined \
					 -export-symbols-regex '^rgxg_' \
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

man_MANS = man/rgxg.1 man/rgxg_number_range.3 \
		   man/rgxg_net_cidr_string.3 man/rgxg_utils_escape_string.3 \
		   man/rgxg_utils_alternation.3

EXTRA_DIST = lib/rgxg.pc.in $(man_MANS)
dist_doc_DATA=AUTHORS COPYING NEWS README

install-data-hook:
	rm -f $(DESTDIR)$(man3dir)/rgxg_number_greaterequal.3
	$(LN_S) rgxg_number_range.3 $(DESTDIR)$(man3dir)/rgxg_number_greaterequal.3
	rm -f $(DESTDIR)$(man3dir)/rgxg_net_cidr_ipv4.3
	$(LN_S) rgxg_net_cidr_string.3 $(DESTDIR)$(man3dir)/rgxg_net_cidr_ipv4.3
	rm -f $(DESTDIR)$(man3dir)/rgxg_net_cidr_ipv6.3
	$(LN_S) rgxg_net_cidr_string.3 $(DESTDIR)$(man3dir)/rgxg_net_cidr_ipv6.3

uninstall-hook:
	rm -f $(DESTDIR)$(man3dir)/rgxg_number_greaterequal.3
	rm -f $(DESTDIR)$(man3dir)/rgxg_net_cidr_ipv4.3
	rm -f $(DESTDIR)$(man3dir)/rgxg_net_cidr_ipv6.3
	rmdir $(DESTDIR)$(docdir) || true
	rmdir $(DESTDIR)$(librgxgincludedir) || true

DISTCLEANFILES = lib/rgxg.pc

autoreconf-clean: maintainer-clean
	-rm aclocal.m4 build-aux/ar-lib build-aux/compile build-aux/config.guess \
	  build-aux/config.sub build-aux/depcomp build-aux/install-sh \
	  build-aux/ltmain.sh build-aux/missing config.h.in config.h.in~ \
	  configure INSTALL m4/libtool.m4 m4/ltoptions.m4 m4/ltsugar.m4 \
	  m4/ltversion.m4 m4/lt~obsolete.m4 m4/version.m4 Makefile.in
	rmdir build-aux/
