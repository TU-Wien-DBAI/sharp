#ifndef SHARP_HTDSOLVER_H_
#define SHARP_HTDSOLVER_H_

#include <libsharp/HtdSolver.hpp>

namespace sharp
{
	struct SHARP_LOCAL HtdSolver::Impl
	{
		Impl(const IHtdAlgorithm &algorithm);

		const IHtdAlgorithm &algorithm;

	}; // struct HtdSolver::Impl

} // namespace sharp

#endif // SHARP_HTDSOLVER_H_
