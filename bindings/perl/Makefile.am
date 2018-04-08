## Makefile.am
##
## Copyright (C) 20108Free Software Foundation, Inc.
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

## bindings/perl/Makefile.am: call SWIG and build perl module
## written by Reini Urban

## TODO: place generated files out of source tree

ACLOCAL_AMFLAGS = -I $(top_srcdir)/m4

## magic to override pkgperldir = $(perldir)/$(PACKAGE)
## at least in Ubuntu perl wouldn't find the module there
#pkgperldir = $(perldir)

if HAVE_SWIG_PERL

BUILT_SOURCES = swig_wrap_perl.c
SWIG_SOURCES = $(srcdir)/../dwg.i $(top_srcdir)/include/dwg.h

dist_perl_PERL = LibreDWG.pm
#plexec_LTLIBRARIES = _libredwg.la

# the underscore is correct. the perl interpreter looks for _libredwg.so
# when libredwg module is loaded
_libredwg_la_SOURCES = $(BUILT_SOURCES)  $(SWIG_SOURCES)

_libredwg_la_CPPFLAGS = -I$(top_srcdir)/include $(AX_SWIG_PERL_CPPFLAGS)
_libredwg_la_LDFLAGS = -module
_libredwg_la_LIBADD = ../../src/libredwg.la

## With macports gcc you need -I/opt/local/lib/gcc6/gcc/x86_64-apple-darwin15/6.4.0/include
swig_wrap_perl.c : $(SWIG_SOURCES) Makefile.am
	swig -perl -I$(top_srcdir)/include -o $@ -importall $(srcdir)/../dwg.i

endif
