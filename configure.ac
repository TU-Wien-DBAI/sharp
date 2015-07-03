#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.65])
AC_INIT([sharp], [2.0], [morak@dbai.tuwien.ac.at])
AC_CONFIG_SRCDIR([src/HtdSolver.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_AUX_DIR([autotools])
AC_CONFIG_FILES([Makefile
		 include/Makefile
		 src/Makefile])

AM_INIT_AUTOMAKE

LT_PREREQ([2.2])
LT_INIT
LT_LANG([C++])

# Checks for programs.
AC_PROG_INSTALL
AC_PROG_LN_S

# Checks for libraries.
AC_ARG_WITH([gmp],
	    [AS_HELP_STRING([--without-gmp], [disable support for GNU MP])],
	    [],
	    [with_gmp=yes])
          
AS_IF([test "x$with_gmp" != xno],
      [
       AC_CHECK_LIB([gmp], [__gmpz_init], [], [AC_MSG_FAILURE([GNU MP library not found, see http://gmplib.org/ (--without-gmp to disable)])])
       AC_CHECK_HEADER([gmp.h], [], [AC_MSG_FAILURE([GNU MP header file not found, see http://gmplib.org/ (--without-gmp to disable)])])
       AC_LANG_PUSH(C++)
       AC_CHECK_HEADER([gmpxx.h], [], [AC_MSG_FAILURE([GNU MP C++ header file not found, see http://gmplib.org/ (--without-gmp to disable)])])
       AC_LANG_POP
      ])

# Checks for C header files.

# Checks for C++ header files

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.
AC_CHECK_FUNCS([strerror])

# Define optional features

# Pass information to automake
AM_CONDITIONAL([WITH_LIBGMP], [test "x$with_gmp" = xyes])

# Definitions for config.h
AC_DEFINE_UNQUOTED([SHARP_EXPORTS], [1], [Define to 1 if the library is being built (as opposed to being used).])

AC_OUTPUT
