## Process this file with automake to produce Makefile.in

docdir = $(datadir)/doc/$(PACKAGE)

EXTRA_DIST = Doxyfile.in

man_MANS =

doc_DATA = doxygen-build.stamp

if HAVE_DOCBOOKTOMAN
%.1: %.1.sgml
	docbook-to-man $< > $@
else
%.1: %.1.sgml
endif

if HAVE_DOCBOOK2HTML
%.html: %.sgml
	mkdir $*
	docbook2html -o $* $<
endif

if HAVE_DOXYGEN
doxygen-build.stamp: Doxyfile $(top_srcdir)/include/blossom/*.h
	doxygen
	touch doxygen-build.stamp
else
doxygen-build.stamp:
	echo "*** Warning: Doxygen not found; documentation will not be built."
	touch doxygen-build.stamp
endif

dist_docdir = $(distdir)/libblossom

dist-hook:
	if test -d libblossom; then \
	  mkdir $(dist_docdir); \
	  for dir in libblossom/*; do \
	    b=`basename $$dir`; \
	    if test $$b != "CVS"; then \
	      if test -d $$dir; then \
	        mkdir $(dist_docdir)/$$b; \
	        for f in $$dir/*; do \
	          cp -p $$f $(dist_docdir)/$$b; \
	        done \
	      fi \
	    fi \
	  done \
	fi


install-data-local: doxygen-build.stamp
	$(mkinstalldirs) $(docdir)
	if test -d libblossom; then \
	  for dir in libblossom/*; do \
	    if test -d $$dir; then \
	      b=`basename $$dir`; \
	      $(mkinstalldirs) $(docdir)/$$b; \
	      for f in $$dir/*; do \
		$(INSTALL_DATA) $$f $(DESTDIR)$(docdir)/$$b; \
	      done \
	    fi \
	  done \
	fi

uninstall-local:
	rm -rf $(DESTDIR)$(docdir);

clean-local:
	if test -d libblossom; then rm -rf libblossom; fi
	if test -f doxygen-build.stamp; then rm -f doxygen-build.stamp; fi
