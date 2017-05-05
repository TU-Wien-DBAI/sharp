#ifdef HAVE_CONFIG_H
	#include <config.h>
#endif

#include "IterativeTreeTupleSolver.hpp"

#include "NodeTupleSetMap.hpp"
#include "TupleSet.hpp"

#include <logic/helpers>

#include <stack>
#include <memory>
#include <cstddef>

namespace sharp
{
	using logic::Benchmark;

	using htd::vertex_t;
	using htd::IHypergraph;
	using htd::ITreeDecomposition;
	using htd::ITreeDecompositionAlgorithm;

	using std::size_t;
	using std::unique_ptr;

	IterativeTreeTupleSolver::IterativeTreeTupleSolver(
			const htd::ITreeDecompositionAlgorithm &decomposer,
			const ITreeTupleAlgorithm &algorithm)
		: IterativeTreeTupleSolver(decomposer, { &algorithm })
	{ }

	IterativeTreeTupleSolver::IterativeTreeTupleSolver(
			const htd::ITreeDecompositionAlgorithm &decomposer,
			const ITreeTupleAlgorithm &algorithm1,
			const ITreeTupleAlgorithm &algorithm2)
		: IterativeTreeTupleSolver(decomposer, { &algorithm1, &algorithm2 })
	{ }

	IterativeTreeTupleSolver::IterativeTreeTupleSolver(
			const htd::ITreeDecompositionAlgorithm &decomposer,
			const TreeTupleAlgorithmVector &algorithms)
		: IterativeTreeSolver(
				decomposer,
				convertAlgorithmList(algorithms),
				true)
	{ }

	IterativeTreeTupleSolver::IterativeTreeTupleSolver(
			const htd::ITreeDecompositionAlgorithm &decomposer,
			const ITreeTupleAlgorithm &algorithm,
			const ITreeTupleSolutionExtractor &extractor)
		: IterativeTreeTupleSolver(decomposer, { &algorithm }, extractor)
	{ }

	IterativeTreeTupleSolver::IterativeTreeTupleSolver(
			const htd::ITreeDecompositionAlgorithm &decomposer,
			const ITreeTupleAlgorithm &alg1,
			const ITreeTupleAlgorithm &alg2,
			const ITreeTupleSolutionExtractor &extractor)
		: IterativeTreeTupleSolver(decomposer, { &alg1, &alg2 }, extractor) { }

	IterativeTreeTupleSolver::IterativeTreeTupleSolver(
			const htd::ITreeDecompositionAlgorithm &decomposer,
			const TreeTupleAlgorithmVector &algorithms,
			const ITreeTupleSolutionExtractor &extractor)
		: IterativeTreeSolver(
				decomposer,
				convertAlgorithmList(algorithms),
				std::unique_ptr<const ITreeSolutionExtractor>(
					new TupleToTreeSolutionExtractorAdapter(extractor)),
				true,
				true)
	{ }

	IterativeTreeTupleSolver::~IterativeTreeTupleSolver() { }

	unique_ptr<INodeTableMap> IterativeTreeTupleSolver::initializeMap(
			size_t decompositionNodeCount) const
	{
		return unique_ptr<INodeTableMap>(
				new NodeTupleSetMap(decompositionNodeCount));
	}

	std::vector<std::unique_ptr<const ITreeAlgorithm> >
	IterativeTreeTupleSolver::convertAlgorithmList(
			const TreeTupleAlgorithmVector &algorithms)
	{
		std::vector<std::unique_ptr<const ITreeAlgorithm> > newAlgorithms;
		for(const ITreeTupleAlgorithm *alg : algorithms)
			newAlgorithms.push_back(
					std::unique_ptr<const ITreeAlgorithm>(
						new TupleToTreeAlgorithmAdapter(*alg)));
		return newAlgorithms;
	}


} // namespace sharp
