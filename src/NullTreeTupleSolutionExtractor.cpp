#ifdef HAVE_CONFIG_H
	#include <config.h>
#endif

#include "NullTreeTupleSolutionExtractor.hpp"

namespace sharp
{
	using htd::vertex_t;
	using htd::ITreeDecomposition;
	
	NullTreeTupleSolutionExtractor::~NullTreeTupleSolutionExtractor() { }

	ISolution *NullTreeTupleSolutionExtractor::extractSolution(
			vertex_t,
			const ITreeDecomposition &,
			const INodeTupleSetMap &,
			const IInstance &) const
	{
		return nullptr;
	}
	
	ISolution *NullTreeTupleSolutionExtractor::emptySolution(
			const IInstance &) const
	{
		return nullptr;
	}

} // namespace sharp
