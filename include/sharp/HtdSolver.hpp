#ifndef SHARP_SHARP_HTDSOLVER_H_
#define SHARP_SHARP_HTDSOLVER_H_

#include <sharp/global.hpp>

#include <sharp/IHtdSolver.hpp>
#include <sharp/IHtdNodeEvaluator.hpp>
#include <sharp/IInstance.hpp>

#include <htd/main>

namespace sharp
{
	class SHARP_API HtdSolver : public virtual IHtdSolver
	{
	protected:
		HtdSolver &operator=(HtdSolver &) { return *this; };

	public:
		HtdSolver(const IHtdNodeEvaluator &evaluator,
				const IHypertreeDecompositionAlgorithm 
					&decomposer);
		virtual ~HtdSolver();

		virtual void solve(const IInstance &instance)
			const;

	private:
		struct Impl;
		Impl *impl;

	}; // class HtdSolver
} // namespace sharp

#endif // SHARP_SHARP_HTDSOLVER_H_
