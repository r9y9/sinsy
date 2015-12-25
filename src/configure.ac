# Process this file with autoconf to produce a configure script.
AC_PREREQ(2.59)
AC_INIT(sinsy, 0.92, http://sinsy.sourceforge.net/)
AC_CONFIG_AUX_DIR([config])
AC_COPYRIGHT(Copyright 2009-2015 Nagoya Institute of Technology)
AM_INIT_AUTOMAKE(subdir-objects)

# Checks for C compiler
AC_PROG_CXX
AM_PROG_CC_C_O
AC_PROG_INSTALL
AC_PROG_RANLIB
AN_MAKEVAR([AR], [AC_PROG_AR])
AN_PROGRAM([ar], [AC_PROG_AR])
AC_DEFUN([AC_PROG_AR], [AC_CHECK_TOOL(AR, ar, :)])
AC_PROG_AR

# Checks for libraries.
AC_CHECK_LIB([m], [log])


# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([stdlib.h string.h])


# Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_TYPE_SIZE_T
AC_CHECK_SIZEOF([char])
AC_CHECK_SIZEOF([unsigned char])
AC_CHECK_SIZEOF([short])
AC_CHECK_SIZEOF([unsigned short])
AC_CHECK_SIZEOF([int])
AC_CHECK_SIZEOF([unsigned int])
AC_CHECK_SIZEOF([long])
AC_CHECK_SIZEOF([unsigned long])
AC_CHECK_SIZEOF([long long])
AC_CHECK_SIZEOF([unsigned long long])

# Checks for library functions.
AC_FUNC_VPRINTF
AC_CHECK_FUNCS([sqrt strchr strrchr strstr])

AC_CANONICAL_HOST
AC_C_BIGENDIAN

# Define HTS_ENGINE_HEADER
AC_ARG_WITH(hts-engine-header-path,
            AS_HELP_STRING([--with-hts-engine-header-path],[specify the hts_engine_API header path]),
            [hts_engine_header_path=$withval],
            [hts_engine_header_path='/usr/local/include'])
if test ! -f "${hts_engine_header_path}/HTS_engine.h"; then
   AC_MSG_ERROR(Cannot find HTS_engine.h)
fi
AC_MSG_CHECKING(for HTS_engine.h)
AC_MSG_RESULT($hts_engine_header_path/HTS_engine.h)
AC_SUBST(HTS_ENGINE_HEADER)
HTS_ENGINE_HEADER=${hts_engine_header_path}/HTS_engine.h

# Define HTS_ENGINE_HEADER_DIR
AC_SUBST(HTS_ENGINE_HEADER_DIR)
HTS_ENGINE_HEADER_DIR=${hts_engine_header_path}

# Define HTS_ENGINE_LIBRARY
AC_ARG_WITH(hts-engine-library-path,
            AS_HELP_STRING([--with-hts-engine-library-path],[specify the hts_engine_API library path]),
            [hts_engine_library_path=$withval],
            [hts_engine_library_path='/usr/local/lib'])
if test ! -f "${hts_engine_library_path}/libHTSEngine.a"; then
   AC_MSG_ERROR(Cannot find libHTSEngine.a)
fi
AC_MSG_CHECKING(for libHTSEngine.a)
AC_MSG_RESULT($hts_engine_library_path/libHTSEngine.a)
AC_SUBST(HTS_ENGINE_LIBRARY)
HTS_ENGINE_LIBRARY=${hts_engine_library_path}/libHTSEngine.a

# Define HTS_ENGINE_LIBRARY_DIR
AC_SUBST(HTS_ENGINE_LIBRARY_DIR)
HTS_ENGINE_LIBRARY_DIR=${hts_engine_library_path}

# Check for windows audio devices
case "$host_os" in
   *win32* | *wince* | *cygwin* | *mingw* )
      AC_HAVE_LIBRARY([winmm],,AC_MSG_ERROR(No winmm))
      ;;
   *)
      ;;
esac

AC_CONFIG_FILES([Makefile lib/Makefile bin/Makefile dic/Makefile])

AC_OUTPUT
