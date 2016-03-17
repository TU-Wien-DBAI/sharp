#ifndef SHARP_SHARP_CREATE_H_
#define SHARP_SHARP_CREATE_H_

#include <sharp/global>

#include <sharp/ITreeSolver.hpp>
#include <sharp/ITreeAlgorithm.hpp>
#include <sharp/ITreeTupleAlgorithm.hpp>
#include <sharp/ITreeSolutionExtractor.hpp>
#include <sharp/ITreeTupleSolutionExtractor.hpp>

#include <htd/main.hpp>

namespace sharp
{
	class SHARP_API create
	{
	public:
		static ITreeSolver *treeSolver(
				const htd::ITreeDecompositionAlgorithm &decomposer,
				const ITreeAlgorithm &algorithm); 

		static ITreeSolver *treeSolver(
				const htd::ITreeDecompositionAlgorithm &decomposer,
				const ITreeAlgorithm &algorithm,
				const ITreeSolutionExtractor &extractor);

		static ITreeSolver *treeSolver(
				const htd::ITreeDecompositionAlgorithm &decomposer,
				const ITreeTupleAlgorithm &algorithm);

		static ITreeSolver *treeSolver(
				const htd::ITreeDecompositionAlgorithm &decomposer,
				const ITreeTupleAlgorithm &algorithm,
				const ITreeTupleSolutionExtractor &extractor);

		static ITreeAlgorithm *treeAlgorithm(
				const ITreeAlgorithm &algorithm1,
				const ITreeAlgorithm &algorithm2);

		static ITreeTupleAlgorithm *treeTupleAlgorithm(
				const ITreeTupleAlgorithm &algorithm1,
				const ITreeTupleAlgorithm &algorithm2);

	private:
		create();

	}; // class create

} // namespace sharp

#endif // SHARP_SHARP_CREATE_H_
