#ifndef SHARP_LIBSHARP_HTDSOLVER_H_
#define SHARP_LIBSHARP_HTDSOLVER_H_

#include <libsharp/global.hpp>

#include <libsharp/IHtdAlgorithm.hpp>
#include <libsharp/IInstance.hpp>
#include <libsharp/IIterativeSolutionBuilder.hpp>
#include <libsharp/EncodedSolution.hpp>
#include <libsharp/ISolutionIterator.hpp>

#include <htdecomp>

namespace sharp
{
	using htd::ITreeDecomposition;

	class SHARP_API HtdSolver
	{
	protected:
		HtdSolver &operator=(HtdSolver &) { return *this; }	

	public:
		HtdSolver(const IHtdAlgorithm &solver);
		~HtdSolver();

		EncodedSolution *computeEncodedSolution(const IInstance &instance, const ITreeDecomposition &decomposition) const;
		ISolutionIterator *computeSolution(const IInstance &instance, const ITreeDecomposition &decomposition, const IIterativeSolutionBuilder &builder) const;

	private:
		struct Impl;
		Impl *impl;

	}; // class HtdSolver

} // namespace sharp

#endif // SHARP_LIBSHARP_HTDSOLVER_H_
