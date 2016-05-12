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
			const ITreeAlgorithm &algorithm1,
			const ITreeAlgorithm &algorithm2)
	{
		return new IterativeTreeSolver(decomposer, algorithm1, algorithm2);
	}

	ITreeSolver *create::treeSolver(
			const ITreeDecompositionAlgorithm &decomposer,
			const TreeAlgorithmVector &algorithms)
	{
		return new IterativeTreeSolver(decomposer, algorithms);
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
			const ITreeAlgorithm &alg1,
			const ITreeAlgorithm &alg2,
			const ITreeSolutionExtractor &extractor)
	{
		return new IterativeTreeSolver(decomposer, alg1, alg2, extractor);
	}

	ITreeSolver *create::treeSolver(
			const ITreeDecompositionAlgorithm &decomposer,
			const TreeAlgorithmVector &algorithms,
			const ITreeSolutionExtractor &extractor)
	{
		return new IterativeTreeSolver(decomposer, algorithms, extractor);
	}

	ITreeSolver *create::treeSolver(
			const ITreeDecompositionAlgorithm &decomposer,
			const ITreeTupleAlgorithm &algorithm)
	{
		return new IterativeTreeTupleSolver(decomposer, algorithm);
	}

	ITreeSolver *create::treeSolver(
			const ITreeDecompositionAlgorithm &decomposer,
			const ITreeTupleAlgorithm &algorithm1,
			const ITreeTupleAlgorithm &algorithm2)
	{
		return new IterativeTreeTupleSolver(decomposer, algorithm1, algorithm2);
	}

	ITreeSolver *create::treeSolver(
			const ITreeDecompositionAlgorithm &decomposer,
			const TreeTupleAlgorithmVector &algorithms)
	{
		return new IterativeTreeTupleSolver(decomposer, algorithms);
	}

	ITreeSolver *create::treeSolver(
			const ITreeDecompositionAlgorithm &decomposer,
			const ITreeTupleAlgorithm &algorithm,
			const ITreeTupleSolutionExtractor &extractor)
	{
		return new IterativeTreeTupleSolver(decomposer, algorithm, extractor);
	}

	ITreeSolver *create::treeSolver(
			const ITreeDecompositionAlgorithm &decomposer,
			const ITreeTupleAlgorithm &alg1,
			const ITreeTupleAlgorithm &alg2,
			const ITreeTupleSolutionExtractor &extractor)
	{
		return new IterativeTreeTupleSolver(decomposer, alg1, alg2, extractor);
	}

	ITreeSolver *create::treeSolver(
			const ITreeDecompositionAlgorithm &decomposer,
			const TreeTupleAlgorithmVector &algorithms,
			const ITreeTupleSolutionExtractor &extractor)
	{
		return new IterativeTreeTupleSolver(decomposer, algorithms, extractor);
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
