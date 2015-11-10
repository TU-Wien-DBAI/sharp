############################
# FLAGS:		   #
############################

# tell autoconf to pass certain flags to automake
#ACLOCAL_AMFLAGS = -I m4 ${ACLOCAL_FLAGS}

# tell automake to pass certain CPPFLAGS to the compiler - in particular, add
# all the test library header paths to the default include path
#AM_CPPFLAGS = -I$(top_srcdir)/include
      	#-I./gtest \
      	#-I./gtest/include

# tell automake to pass certain flags to 'ctags'
#AM_CTAGSFLAGS = --extra=+f

############################
# TESTING LIBRARIES:  	   #
############################

#check_LIBRARIES = gtest/libgtest.a
#
#GTEST_OTHER_SRC=gtest/src/gtest-filepath.cc \
#	gtest/src/gtest.cc \
#	gtest/src/gtest_main.cc \
#	gtest/src/gtest-printers.cc \
#	gtest/src/gtest-test-part.cc \
#	gtest/src/gtest-death-test.cc \
#	gtest/src/gtest-port.cc \
#	gtest/src/gtest-typed-test.cc \
#	gtest/src/gtest-internal-inl.h \
#	gtest/include/gtest/gtest-test-part.h \
#	gtest/include/gtest/gtest.h \
#	gtest/include/gtest/gtest-death-test.h \
#	gtest/include/gtest/gtest-param-test.h.pump \
#	gtest/include/gtest/gtest-spi.h \
#	gtest/include/gtest/internal \
#	gtest/include/gtest/internal/gtest-type-util.h.pump \
#	gtest/include/gtest/internal/gtest-param-util.h \
#	gtest/include/gtest/internal/gtest-internal.h \
#	gtest/include/gtest/internal/gtest-string.h \
#	gtest/include/gtest/internal/gtest-filepath.h \
#	gtest/include/gtest/internal/gtest-param-util-generated.h \
#	gtest/include/gtest/internal/gtest-param-util-generated.h.pump \
#	gtest/include/gtest/internal/gtest-death-test-internal.h \
#	gtest/include/gtest/internal/gtest-linked_ptr.h \
#	gtest/include/gtest/internal/gtest-type-util.h \
#	gtest/include/gtest/internal/gtest-tuple.h \
#	gtest/include/gtest/internal/gtest-port.h \
#	gtest/include/gtest/internal/gtest-tuple.h.pump \
#	gtest/include/gtest/gtest_prod.h \
#	gtest/include/gtest/gtest_pred_impl.h \
#	gtest/include/gtest/gtest-printers.h \
#	gtest/include/gtest/gtest-param-test.h \
#	gtest/include/gtest/gtest-message.h \
#	gtest/include/gtest/gtest-typed-test.h
#
#gtest_libgtest_a_SOURCES = \
#	gtest/src/gtest-filepath.cc \
#	gtest/src/gtest.cc \
#	gtest/src/gtest_main.cc \
#	gtest/src/gtest-printers.cc \
#	gtest/src/gtest-test-part.cc \
#	gtest/src/gtest-all.cc \
#	gtest/src/gtest-death-test.cc \
#	gtest/src/gtest-port.cc \
#	gtest/src/gtest-typed-test.cc \
#	gtest/src/gtest-internal-inl.h \
#	gtest/include/gtest/gtest-test-part.h \
#	gtest/include/gtest/gtest.h \
#	gtest/include/gtest/gtest-death-test.h \
#	gtest/include/gtest/gtest-param-test.h.pump \
#	gtest/include/gtest/gtest-spi.h \
#	gtest/include/gtest/internal \
#	gtest/include/gtest/internal/gtest-type-util.h.pump \
#	gtest/include/gtest/internal/gtest-param-util.h \
#	gtest/include/gtest/internal/gtest-internal.h \
#	gtest/include/gtest/internal/gtest-string.h \
#	gtest/include/gtest/internal/gtest-filepath.h \
#	gtest/include/gtest/internal/gtest-param-util-generated.h \
#	gtest/include/gtest/internal/gtest-param-util-generated.h.pump \
#	gtest/include/gtest/internal/gtest-death-test-internal.h \
#	gtest/include/gtest/internal/gtest-linked_ptr.h \
#	gtest/include/gtest/internal/gtest-type-util.h \
#	gtest/include/gtest/internal/gtest-tuple.h \
#	gtest/include/gtest/internal/gtest-port.h \
#	gtest/include/gtest/internal/gtest-tuple.h.pump \
#	gtest/include/gtest/gtest_prod.h \
#	gtest/include/gtest/gtest_pred_impl.h \
#	gtest/include/gtest/gtest-printers.h \
#	gtest/include/gtest/gtest-param-test.h \
#	gtest/include/gtest/gtest-message.h \
#	gtest/include/gtest/gtest-typed-test.h

############################
# TESTS TO BUILD:	   #
############################

# To all tests, link the library that we want to test (libhtd.la), and all the
# test libraries we want to use for writing unit tests (see above section).
#LDADD = $(top_srcdir)/libhtd.la gtest/libgtest.a

# tell automake which test binaries to build
#check_PROGRAMS = test1 test2

# tell automake that for each program listed in PROGRAMS above, if no SOURCES
# are given it should try and build it from the single source file <prog>.cpp,
# where <prog> is the name of the program.
#AM_DEFAULT_SOURCE_EXT = .cpp

# list all source code files for test1 binary
#test1_SOURCES = main.cpp \
#	Exporter.cpp \
#	InstanceReader.cpp \
#	TestHelpers.cpp \
#	Tests.cpp

# tell autotools which binaries/scripts to run for testing
#TESTS = $(check_PROGRAMS)
