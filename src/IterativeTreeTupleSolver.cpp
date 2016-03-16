#ifdef HAVE_CONFIG_H
	#include <config.h>
#endif

#include "IterativeTreeTupleSolver.hpp"

#include "NodeTupleSetMap.hpp"
#include "TupleSet.hpp"

#include <sharp/Benchmark.hpp>

#include <stack>
#include <memory>
#include <cstddef>

namespace sharp
{
	using htd::vertex_t;
	using htd::IHypergraph;
	using htd::ITreeDecomposition;
	using htd::ITreeDecompositionAlgorithm;

	using std::size_t;
	using std::unique_ptr;

	IterativeTreeTupleSolver::IterativeTreeTupleSolver(
			const ITreeDecompositionAlgorithm &decomposer,
			const ITreeTupleAlgorithm &algorithm)
		: IterativeTreeSolver(
				decomposer,
				std::move(unique_ptr<const ITreeAlgorithm>(
						new TupleToTreeAlgorithmAdapter(algorithm))))
	{ }

	IterativeTreeTupleSolver::IterativeTreeTupleSolver(
			const ITreeDecompositionAlgorithm &decomposer,
			const ITreeTupleAlgorithm &algorithm,
			const ITreeTupleSolutionExtractor &extractor)
		: IterativeTreeSolver(
				decomposer,
				std::move(unique_ptr<const ITreeAlgorithm>(
						new TupleToTreeAlgorithmAdapter(algorithm))),
				std::move(unique_ptr<const ITreeSolutionExtractor>(
						new TupleToTreeSolutionExtractorAdapter(extractor))))
	{ }

	IterativeTreeTupleSolver::~IterativeTreeTupleSolver() { }

	unique_ptr<INodeTableMap> IterativeTreeTupleSolver::initializeMap(
			size_t decompositionNodeCount) const
	{
		return unique_ptr<INodeTableMap>(
				new NodeTupleSetMap(decompositionNodeCount));
	}

} // namespace sharp
