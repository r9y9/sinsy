
AM_CPPFLAGS = -I @top_srcdir@/include

bin_PROGRAMS = sinsy 

sinsy_SOURCES = sinsy.cpp

sinsy_LDADD = @top_srcdir@/lib/libSinsy.a \
              @HTS_ENGINE_LIBRARY@

DISTCLEANFILES = *.log *.out *~

MAINTAINERCLEANFILES = Makefile.in 
