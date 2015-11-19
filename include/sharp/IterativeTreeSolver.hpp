#ifndef SHARP_SHARP_ITERATIVETREESOLVER_H_
#define SHARP_SHARP_ITERATIVETREESOLVER_H_

#include <sharp/global>

#include <sharp/ISolver.hpp>
#include <sharp/ITreeEvaluator.hpp>
#include <sharp/IInstance.hpp>
#include <sharp/ISolution.hpp>

#include <htd/main>

namespace sharp
{
	class SHARP_API IterativeTreeSolver : public virtual ISolver
	{
	protected:
		IterativeTreeSolver &operator=(IterativeTreeSolver &)
		{ return *this; };

	public:
		IterativeTreeSolver(
				const htd::ITreeDecompositionAlgorithm &decomposer,
				const ITreeEvaluator &evaluator);

		virtual ~IterativeTreeSolver();

		virtual ISolution *solve(const IInstance &instance) const;


		class Impl;
	private:
		Impl * const impl;

	}; // class IterativeTreeSolver
} // namespace sharp

#endif // SHARP_SHARP_ITERATIVETREESOLVER_H_
