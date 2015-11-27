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
	include/sharp/ConstEnumerator.hpp \
	include/sharp/Enumerator.hpp \
	include/sharp/EnumeratorSkeleton.hpp \
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
	include/sharp/IterativeTreeTupleSolver.hpp

# list all source code files for the libsharp.la library
libsharp_la_SOURCES = \
	src/assembly.cpp \
	\
	src/ITuple.hpp \
	\
	src/IterativeTreeSolverBase.cpp \
	src/IterativeTreeSolverBase.hpp \
	src/IterativeTreeSolver.cpp \
	src/IterativeTreeSolver.hpp \
	src/IterativeTreeTupleSolver.cpp \
	src/IterativeTreeTupleSolver.hpp \
	src/NodeTableMap.cpp \
	src/NodeTableMap.hpp \
	src/NodeTupleSetMap.cpp \
	src/NodeTupleSetMap.hpp \
	src/NullTreeSolutionExtractor.cpp\
	src/NullTreeSolutionExtractor.hpp\
	src/NullTreeTupleSolutionExtractor.cpp\
	src/NullTreeTupleSolutionExtractor.hpp\
	src/TupleSet.cpp \
	src/TupleSet.hpp \
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
# 	CURRENT: This is the version of the (external) ABI in the library.
# 		Increase if the ABI changes (i.e. interfaces added or changed)!
#
# 	REVISION: The revision represents how often the library was changed for a
# 		given ABI version. Increase this whenever the library code changes, but
# 		reset to 0 when incrementing CURRENT.
#
# 	AGE: This is the difference between the oldest and newest ABI version
# 		that the library is backwards compatible with (e.g. if CURRENT is 4,
# 		but we are backwards compatible to ABI version 1, then AGE = 3). Set to
# 		0 if there is a backwards-compatibility breaking change, otherwise
# 		increase by 1 if CURRENT was increased by one.
#
libsharp_la_LDFLAGS = -version-info 1:0:0

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
