#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#include "IterativeTreeSolver.hpp"
#include "IterativeTreeTupleSolver.hpp"
//#include "TwoStepTreeAlgorithmAdapter.hpp"
//#include "TwoStepTreeTupleAlgorithmAdapter.hpp"

#include <sharp/create.hpp>

namespace sharp
{
	using htd::ITreeDecompositionAlgorithm;

	ITreeSolver *treeSolver(
			const ITreeDecompositionAlgorithm &decomposer,
			const ITreeAlgorithm &algorithm)
	{
		return new IterativeTreeSolver(decomposer, algorithm);
	}

	ITreeSolver *treeSolver(
			const ITreeDecompositionAlgorithm &decomposer,
			const ITreeAlgorithm &algorithm,
			const ITreeSolutionExtractor &extractor)
	{
		return new IterativeTreeSolver(decomposer, algorithm, extractor);
	}

	ITreeSolver *treeSolver(
			const ITreeDecompositionAlgorithm &decomposer,
			const ITreeTupleAlgorithm &algorithm)
	{
		return new IterativeTreeTupleSolver(decomposer, algorithm);
	}

	ITreeSolver *treeSolver(
			const ITreeDecompositionAlgorithm &decomposer,
			const ITreeTupleAlgorithm &algorithm,
			const ITreeTupleSolutionExtractor &extractor)
	{
		return new IterativeTreeTupleSolver(decomposer, algorithm, extractor);
	}

	ITreeAlgorithm *treeAlgorithm(
			const ITreeAlgorithm &algorithm1,
			const ITreeAlgorithm &algorithm2)
	{
		//TODO
		//return new TwoStepTreeAlgorithmAdapter(algorithm1, algorithm2);
		return nullptr;
	}

	ITreeTupleAlgorithm *treeTupleAlgorithm(
			const ITreeTupleAlgorithm &algorithm1,
			const ITreeTupleAlgorithm &algorithm2)
	{
		//TODO
		//return new TwoStepTreeTupleAlgorithmAdapter(algorithm1, algorithm2);
		return nullptr;
	}

} // namespace sharp
