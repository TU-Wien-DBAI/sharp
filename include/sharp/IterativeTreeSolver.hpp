#ifndef SHARP_SHARP_ITERATIVETREESOLVER_H_
#define SHARP_SHARP_ITERATIVETREESOLVER_H_

#include <sharp/global>

#include <sharp/ISolver.hpp>
#include <sharp/ITreeAlgorithm.hpp>
#include <sharp/IInstance.hpp>
#include <sharp/ISolution.hpp>
#include <sharp/ITreeSolutionExtractor.hpp>

#include <htd/main>

namespace sharp
{
	class SHARP_API IterativeTreeSolver : public ISolver
	{
	protected:
		IterativeTreeSolver &operator=(IterativeTreeSolver &)
		{ return *this; };

	public:
		IterativeTreeSolver(
				const htd::ITreeDecompositionAlgorithm &decomposer,
				const ITreeAlgorithm &calculator);

		IterativeTreeSolver(
				const htd::ITreeDecompositionAlgorithm &decomposer,
				const ITreeAlgorithm &calculator,
				const ITreeSolutionExtractor &extractor);

		virtual ~IterativeTreeSolver();

		virtual ISolution *solve(const IInstance &instance) const;

	private:
		class Impl;
		Impl * const impl;

	}; // class IterativeTreeSolver
} // namespace sharp

#endif // SHARP_SHARP_ITERATIVETREESOLVER_H_
