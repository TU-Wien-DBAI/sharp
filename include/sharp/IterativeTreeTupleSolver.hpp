#ifndef SHARP_SHARP_ITERATIVETREETUPLESOLVER_H_
#define SHARP_SHARP_ITERATIVETREETUPLESOLVER_H_

#include <sharp/global>

#include <sharp/IterativeTreeSolver.hpp>
#include <sharp/ITreeTupleAlgorithm.hpp>
#include <sharp/ITreeTupleSolutionExtractor.hpp>

#include <htd/main>

namespace sharp
{
	class SHARP_API IterativeTreeTupleSolver
	{
	protected:
		IterativeTreeTupleSolver &operator=(IterativeTreeTupleSolver &)
		{ return *this; };

	public:
		IterativeTreeTupleSolver(
				const htd::ITreeDecompositionAlgorithm &decomposer,
				const ITreeTupleAlgorithm &algorithm);

		IterativeTreeTupleSolver(
				const htd::ITreeDecompositionAlgorithm &decomposer,
				const ITreeTupleAlgorithm &algorithm,
				const ITreeTupleSolutionExtractor &extractor);

		virtual ~IterativeTreeTupleSolver();

		virtual ISolution *solve(const IInstance &instance) const;

	private:
		class Impl;
		Impl * const impl;

	}; // class IterativeTreeTupleSolver
} // namespace sharp

#endif // SHARP_SHARP_ITERATIVETREETUPLESOLVER_H_
