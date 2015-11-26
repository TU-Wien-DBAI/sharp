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
	-pedantic-errors \
	-Wpedantic \
	-Wall \
	-Wextra \
	-Wdouble-promotion \
	-Wswitch-default \
	-fvisibility=hidden \
	-fvisibility-inlines-hidden

############################
# LIBRARIES TO BUILD:	   #
############################

# tell automake which libraries to build
lib_LTLIBRARIES = libsharp.la

#--------------------------#
# libsharp.la build options  #
#--------------------------#
	
# list the names of the public header files of the libsharp.la library
libsharp_la_include_HEADERS = \
	include/sharp/main \
	include/sharp/global \
	\
	include/sharp/IEnumerator.hpp \
	include/sharp/IInstance.hpp \
	include/sharp/IMutableNodeTableMap.hpp \
	include/sharp/IMutableNodeTupleSetMap.hpp \
	include/sharp/INodeTableMap.hpp \
	include/sharp/INodeTupleSetMap.hpp \
	include/sharp/ISolution.hpp \
	include/sharp/ISolver.hpp \
	include/sharp/ITable.hpp \
	include/sharp/ITreeAlgorithm.hpp \
	include/sharp/ITreeSolutionExtractor.hpp \
	include/sharp/ITreeTupleAlgorithm.hpp \
	include/sharp/ITreeTupleSolutionExtractor.hpp \
	include/sharp/ITuple.hpp \
	include/sharp/ITupleSet.hpp \
	\
	include/sharp/IterativeTreeSolver.hpp

# list all source code files for the libsharp.la library
libsharp_la_SOURCES = \
	src/assembly.cpp \
	\
	src/ITuple.hpp \
	\
	src/IterativeTreeSolver.cpp \
	src/IterativeTreeSolver.hpp \
	src/NodeTableMap.cpp \
	src/NodeTableMap.hpp \
	src/NodeTupleSetMap.cpp \
	src/NodeTupleSetMap.hpp \
	src/NullTreeSolutionExtractor.cpp\
	src/NullTreeSolutionExtractor.hpp\
	\
	src/util/errorhandling.cpp \
	src/util/errorhandling.hpp

# tell automake that the installation directory for public header files of the
# libsharp.la library should be a subfolder in the default ${includedir}
libsharp_la_includedir = ${includedir}/sharp

# tell libtool to pass certain LDFLAGS to the compiler when building libsharp.la
#
# 1) Specify the Shared Object Version Information for the library, which
# consists of three numbers: CURRENT:REVISION:AGE
#
# 	CURRENT: This is the version of the (external) API in the library.
# 		Increase if the API changes (i.e. interface added or changed)!
#
# 	REVISION: The revision represents how often the library was changed.
# 		Increase this whenever the library code changes (always)!
#
# 	AGE: This is the difference between the oldest and newest API version
# 		that the library supports (e.g. if API version is 4, but we
# 		are backwards compatible to API version 1, then AGE = 3).
#
#FIXME: do feature detection
libsharp_la_LDFLAGS = -version-info 1:1:0

############################
# DISTRIBUTION OPTIONS:	   #
############################

# tell automake that the files after the "=" should not be installed into a
# distribution tarball of the library.
dist_noinst_SCRIPTS = autogen.sh

# tell automake which files to package into distribution tarballs not involved
# in rules or autoconf templates
EXTRA_DIST = 

############################
# UNIT TESTS:              #
############################

# tell automake to recursively go into the test directory
SUBDIRS = tests
