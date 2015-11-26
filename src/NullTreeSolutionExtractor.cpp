#ifdef HAVE_CONFIG_H
	#include <config.h>
#endif

#include "NullTreeSolutionExtractor.hpp"

namespace sharp
{
	using htd::vertex_t;
	using htd::ITreeDecomposition;
	
	NullTreeSolutionExtractor::~NullTreeSolutionExtractor() { }

	ISolution *NullTreeSolutionExtractor::extractSolution(
			vertex_t,
			const ITreeDecomposition &,
			const INodeTableMap &,
			const IInstance &) const
	{
		return nullptr;
	}
	
	ISolution *NullTreeSolutionExtractor::emptySolution(
			const IInstance &) const
	{
		return nullptr;
	}

} // namespace sharp
