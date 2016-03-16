#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#include "IterativeTreeTupleSolver.hpp"

namespace sharp
{
	using htd::vertex_t;
	using htd::ITreeDecomposition;
	using htd::ILabelingFunction;

	IterativeTreeTupleSolver::
	TupleToTreeSolutionExtractorAdapter::TupleToTreeSolutionExtractorAdapter(
			const ITreeTupleSolutionExtractor &extractor)
		: extractor_(extractor)
	{ }

	IterativeTreeTupleSolver::
	TupleToTreeSolutionExtractorAdapter::~TupleToTreeSolutionExtractorAdapter()
	{ }

	ISolution *
	IterativeTreeTupleSolver::
	TupleToTreeSolutionExtractorAdapter::extractSolution(
			vertex_t node,
			const ITreeDecomposition &decomposition,
			const INodeTableMap &tables,
			const IInstance &instance) const
	{
		return extractor_.extractSolution(
				node,
				decomposition,
				dynamic_cast<const INodeTupleSetMap &>(tables),
				instance);
	}

	ISolution *
	IterativeTreeTupleSolver::
	TupleToTreeSolutionExtractorAdapter::emptySolution(
			const IInstance &instance) const
	{
		return extractor_.emptySolution(instance);
	}

} // namespace sharp
