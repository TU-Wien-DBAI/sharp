AM_CPPFLAGS = -I$(top_srcdir)/include --std=c++11 --pedantic-errors -Wpedantic -Wall -Wextra -Wdouble-promotion -Wswitch-default -fvisibility=hidden -fvisibility-inlines-hidden
AM_CTAGSFLAGS = --extra=+f

lib_LTLIBRARIES = libsharp.la
libsharp_la_SOURCES = ./EncodedSolution.cpp \
./EncodedSolution.hpp \
./HtdSolutionTreeBuilder.cpp \
./HtdSolutionTreeBuilder.hpp \
./HtdSolutionTree.cpp \
./HtdSolutionTree.hpp \
./HtdSolver.cpp \
./HtdSolver.hpp \
./util/errorhandling.cpp \
./util/errorhandling.hpp

libsharp_la_LDFLAGS = -version-info 1:0:0
