#ifndef SHARP_SHARP_ITERATIVETREETUPLESOLVER_H_
#define SHARP_SHARP_ITERATIVETREETUPLESOLVER_H_

#include <sharp/global>

#include <sharp/ISolver.hpp>
#include <sharp/IInstance.hpp>
#include <sharp/ITreeTupleAlgorithm.hpp>
#include <sharp/ITreeTupleSolutionExtractor.hpp>

#include <htd/main.hpp>

#include <cstddef>

namespace sharp
{
	class SHARP_API IterativeTreeTupleSolver : public ISolver
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

		std::size_t calculateTreewidth(const IInstance &instance) const;

		virtual ISolution *solve(const IInstance &instance) const;

	private:
		class Impl;
		Impl * const impl;

	}; // class IterativeTreeTupleSolver

} // namespace sharp

#endif // SHARP_SHARP_ITERATIVETREETUPLESOLVER_H_
