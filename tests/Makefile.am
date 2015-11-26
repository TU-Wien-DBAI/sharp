############################
# FLAGS:                   #
############################

# tell automake to pass certain flags to autoconf on re-run
ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}

# tell automake to pass certain flags to 'ctags'
AM_CTAGSFLAGS = --extra=+f

#FIXME: don't assume GCC, do feature detection in configure.ac
# tell automake to pass certain CPPFLAGS to the compiler
AM_CPPFLAGS = \
	-I$(top_srcdir)/include \
	-Ilib/gtest/include \
	-Ilib/gtest \
	-pedantic-errors \
	-Wpedantic \
	-Wall \
	-Wextra \
	-Wdouble-promotion \
	-Wswitch-default

# add pthread flags (required by gtest)
AM_CXXFLAGS = $(PTHREAD_CFLAGS)
AM_CPPFLAGS += -DGTEST_HAS_PTHREAD=1

# add pthread libs (required by gtest)
LDADD = $(PTHREAD_LIBS)

############################
# LIBRARIES TO BUILD:	   #
############################

# tell automake which static libraries to build
check_LTLIBRARIES = \
	libgtest.la \
	libgtest_main.la

# List all files of gtest which are not compiled directly. They thus need to
# be added to the EXTRA_DIST files, to be included distributions.
GTEST_FILES = \
	lib/gtest/LICENSE \
	lib/gtest/include/lib/gtest/lib/gtest-death-test.h \
	lib/gtest/include/lib/gtest/lib/gtest-message.h \
	lib/gtest/include/lib/gtest/lib/gtest-param-test.h \
	lib/gtest/include/lib/gtest/lib/gtest-printers.h \
	lib/gtest/include/lib/gtest/lib/gtest-spi.h \
	lib/gtest/include/lib/gtest/lib/gtest-test-part.h \
	lib/gtest/include/lib/gtest/lib/gtest-typed-test.h \
	lib/gtest/include/lib/gtest/lib/gtest.h \
	lib/gtest/include/lib/gtest/lib/gtest_pred_impl.h \
	lib/gtest/include/lib/gtest/lib/gtest_prod.h \
	lib/gtest/include/lib/gtest/internal/lib/gtest-death-test-internal.h \
	lib/gtest/include/lib/gtest/internal/lib/gtest-filepath.h \
	lib/gtest/include/lib/gtest/internal/lib/gtest-internal.h \
	lib/gtest/include/lib/gtest/internal/lib/gtest-linked_ptr.h \
	lib/gtest/include/lib/gtest/internal/lib/gtest-param-util-generated.h \
	lib/gtest/include/lib/gtest/internal/lib/gtest-param-util.h \
	lib/gtest/include/lib/gtest/internal/lib/gtest-port.h \
	lib/gtest/include/lib/gtest/internal/lib/gtest-string.h \
	lib/gtest/include/lib/gtest/internal/lib/gtest-tuple.h \
	lib/gtest/include/lib/gtest/internal/lib/gtest-type-util.h \
	lib/gtest/src/lib/gtest-death-test.cc \
	lib/gtest/src/lib/gtest-filepath.cc \
	lib/gtest/src/lib/gtest-internal-inl.h \
	lib/gtest/src/lib/gtest-port.cc \
	lib/gtest/src/lib/gtest-printers.cc \
	lib/gtest/src/lib/gtest-test-part.cc \
	lib/gtest/src/lib/gtest-typed-test.cc \
	lib/gtest/src/lib/gtest.cc

#--------------------------#
# libgtest.a build options #
#--------------------------#
	
# list all source code files for the libgtest.a library
libgtest_la_SOURCES = lib/gtest/src/gtest-all.cc

#-------------------------------#
# libgtest_main.a build options #
#-------------------------------#

# list all source code files for the libgtest_main.a library
libgtest_main_la_SOURCES = lib/gtest/src/gtest_main.cc

# list all libraries needed to build libgtest_main.a
libgtest_main_la_LIBADD = libgtest.la

############################
# TESTS TO BUILD           #
############################

# List all files containing mock classes. Those are not compiled directly and
# need to be added to the EXTRA_DIST files, to be included distributions.
MOCK_FILES = \
	mocks/MockTreeEvaluator.cpp

# To all tests, link the library that we want to test (libhtd.la), and all the
# test libraries we want to use for writing unit tests (see above section).
LDADD += $(top_srcdir)/libsharp.la libgtest.la libgtest_main.la

# tell automake which test binaries to build
check_PROGRAMS = \
	integration/IterativeTreeSolver

# tell automake that for each program listed in PROGRAMS above, if no SOURCES
# are given it should try and build it from the single source file <prog>.cpp,
# where <prog> is the name of the program.
AM_DEFAULT_SOURCE_EXT = .cpp

# tell autotools which binaries/scripts to run for testing
TESTS = $(check_PROGRAMS)

############################
# DISTRIBUTION OPTIONS:	   #
############################

# tell automake which files to package into distribution tarballs not involved
# in rules or autoconf templates (here: gtest sources not directly built))
EXTRA_DIST = $(GTEST_FILES) $(MOCK_FILES) 
