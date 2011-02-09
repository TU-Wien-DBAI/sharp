#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.68])
AC_INIT([sharp], [0.1], [morak@dbai.tuwien.ac.at])
AM_INIT_AUTOMAKE([sharp], [0.1])
LT_PREREQ([2.4])
LT_INIT([dlopen])
AC_CONFIG_SRCDIR([src/Problem.cpp])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_MACRO_DIR([m4])
AC_CONFIG_FILES([Makefile
		 include/Makefile
		 src/Makefile])

# Checks for programs.
AC_PROG_CXX

# Checks for libraries.

# Checks for header files.

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL

# Checks for library functions.
AC_CHECK_FUNCS([strerror])

AC_OUTPUT
