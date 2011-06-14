AM_CTAGSFLAGS = --extra=+f

libsharp_la_includedir = ${pkgincludedir}
libsharp_la_include_HEADERS = ./sharp/AbstractAlgorithm.hpp \
			      ./sharp/AbstractEliminationOrdering.hpp \
			      ./sharp/AbstractHypertreeDecompositionAlgorithm.hpp \
			      ./sharp/BucketEliminationAlgorithm.hpp \
			      ./sharp/Component.hpp \
			      ./sharp/CompSet.hpp \
			      ./sharp/ExtendedHypertree.hpp \
			      ./sharp/Global.hpp \
			      ./sharp/Globals.hpp \
			      ./sharp/H_BucketElim.hpp \
			      ./sharp/Hyperedge.hpp \
			      ./sharp/Hypergraph.hpp \
			      ./sharp/Hypertree.hpp \
			      ./sharp/MaximumCardinalitySearchOrdering.hpp \
			      ./sharp/MinimumDegreeOrdering.hpp \
			      ./sharp/MinimumFillOrdering.hpp \
			      ./sharp/Node.hpp \
			      ./sharp/Parser.hpp \
			      ./sharp/Problem.hpp \
			      ./sharp/probSol.hpp \
			      ./sharp/SetCover.hpp \
			      ./sharp/SetCoverLS.hpp \
			      ./sharp/SpecialHEdge.hpp \
			      ./sharp/support.hpp \
			      ./sharp/TabuSearch.hpp \
			      ./sharp/main

noinst_HEADERS = ./sharp/GMPCountingSolutionContent.hpp \
		 ./sharp/NonGMPCountingSolutionContent.hpp

nodist_libsharp_la_include_HEADERS = ./sharp/CountingSolutionContent.hpp

BUILT_SOURCES = ./sharp/CountingSolutionContent.hpp.phony

if USE_LIBGMP
./sharp/CountingSolutionContent.hpp.phony:
	cmp ./sharp/GMPCountingSolutionContent.hpp ./sharp/CountingSolutionContent.hpp >/dev/null 2>&1 \
		|| cp ./sharp/GMPCountingSolutionContent.hpp ./sharp/CountingSolutionContent.hpp
else
./sharp/CountingSolutionContent.hpp.phony:
	cmp ./sharp/NonGMPCountingSolutionContent.hpp ./sharp/CountingSolutionContent.hpp >/dev/null 2>&1 \
		|| cp ./sharp/NonGMPCountingSolutionContent.hpp ./sharp/CountingSolutionContent.hpp
endif
