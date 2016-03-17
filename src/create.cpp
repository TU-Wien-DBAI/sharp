#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#include "IterativeTreeSolver.hpp"
#include "IterativeTreeTupleSolver.hpp"
#include "InterleavedTreeAlgorithm.hpp"
#include "InterleavedTreeTupleAlgorithm.hpp"

#include <sharp/create.hpp>

namespace sharp
{
	using htd::ITreeDecompositionAlgorithm;

	ITreeSolver *create::treeSolver(
			const ITreeDecompositionAlgorithm &decomposer,
			const ITreeAlgorithm &algorithm)
	{
		return new IterativeTreeSolver(decomposer, algorithm);
	}

	ITreeSolver *create::treeSolver(
			const ITreeDecompositionAlgorithm &decomposer,
			const ITreeAlgorithm &algorithm,
			const ITreeSolutionExtractor &extractor)
	{
		return new IterativeTreeSolver(decomposer, algorithm, extractor);
	}

	ITreeSolver *create::treeSolver(
			const ITreeDecompositionAlgorithm &decomposer,
			const ITreeTupleAlgorithm &algorithm)
	{
		return new IterativeTreeTupleSolver(decomposer, algorithm);
	}

	ITreeSolver *create::treeSolver(
			const ITreeDecompositionAlgorithm &decomposer,
			const ITreeTupleAlgorithm &algorithm,
			const ITreeTupleSolutionExtractor &extractor)
	{
		return new IterativeTreeTupleSolver(decomposer, algorithm, extractor);
	}

	ITreeAlgorithm *create::treeAlgorithm(
			const ITreeAlgorithm &algorithm1,
			const ITreeAlgorithm &algorithm2)
	{
		return new InterleavedTreeAlgorithm(algorithm1, algorithm2);
	}

	ITreeTupleAlgorithm *create::treeTupleAlgorithm(
			const ITreeTupleAlgorithm &algorithm1,
			const ITreeTupleAlgorithm &algorithm2)
	{
		return new InterleavedTreeTupleAlgorithm(algorithm1, algorithm2);
	}

} // namespace sharp
