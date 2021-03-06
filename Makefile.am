## Makefile.am
##
## Copyright (C) 2010 Free Software Foundation, Inc.
##
## This file is part of LibreDWG.
##
## LibreDWG is free software: you can redistribute it and/or modify it
## under the terms of the GNU General Public License as published by
## the Free Software Foundation, either version 3 of the License, or
## (at your option) any later version.
##
## LibreDWG is distributed in the hope that it will be useful,
## but WITHOUT ANY WARRANTY; without even the implied warranty
## of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
## See the GNU General Public License for more details.
##
## You should have received a copy of the GNU General Public License
## along with this program.  If not, see <http://www.gnu.org/licenses/>.

ACLOCAL_AMFLAGS = -I m4

SUBDIRS = src programs examples test doc
if HAVE_SWIG_PYTHON
SUBDIRS += bindings
endif

include_HEADERS = \
	include/dwg.h \
	include/dwg_api.h

.PHONY: check-dwg check-dwg-valgrind man scan-build gcov check-wine

man:
	$(MAKE) -C programs man
check-dwg: all
	-rm check-dwg.log
	for d in `find test/test-data -iname \*.dwg`; do \
	  echo programs/dwgread -v3 $$d | tee -a check-dwg.log; \
	  programs/dwgread -v3 $$d >> check-dwg.log 2>&1; \
	done
check-dwg-valgrind: all
	-rm check-dwg-valgrind.log
	for d in `find test/test-data -iname \*.dwg`; do \
	  echo valgrind --leak-check=full programs/dwgread -v3 $$d | tee -a check-dwg-valgrind.log; \
	  valgrind --leak-check=full programs/dwgread -v3 $$d >> check-dwg-valgrind.log 2>&1; \
	done

# clang-analyzer.llvm.org
SCAN_BUILD = scan-build
scan-build: clean
	$(SCAN_BUILD) -V -o .analysis $(MAKE) -j4

if ENABLE_GCOV
gcov: check
	$(MAKE) -C test gcov
	$(MAKE) -C src gcov
	gcov2perl test/*/*.gcov src/*.gcov
	cover -no-gcov
endif

EXTRA_DIST = README README-alpha
CLEANFILES = check-dwg.log

maintainer-clean-local:
	-rm -rf cover_db
LIBTOOL_DISTCLEAN_FILES = \
	m4/libtool.m4 m4/ltoptions.m4 m4/ltsugar.m4 m4/ltversion.m4 m4/lt~obsolete.m4 \
	build-aux/compile \
	build-aux/config.guess \
	build-aux/config.sub \
	build-aux/depcomp \
	build-aux/install-sh \
	build-aux/ltmain.sh \
	build-aux/missing \
	build-aux/py-compile \
	build-aux/textinfo.tex
