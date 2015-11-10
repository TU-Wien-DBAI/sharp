#ifndef SHARP_HTDSOLVER_H_
#define SHARP_HTDSOLVER_H_

#include <sharp/HtdSolver.hpp>

namespace sharp
{
	struct SHARP_LOCAL HtdSolver::Impl
	{
		Impl(const IHtdNodeEvaluator &evaluator);

		const IHtdNodeEvaluator &evaluator;

	}; // struct HtdSolver::Impl

} // namespace sharp

#endif // SHARP_HTDSOLVER_H_
