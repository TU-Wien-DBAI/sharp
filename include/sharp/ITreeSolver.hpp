#ifndef SHARP_SHARP_ITREESOLVER_H_
#define SHARP_SHARP_ITREESOLVER_H_

#include <sharp/global>

#include <sharp/ISolver.hpp>

#include <htd/main.hpp>

namespace sharp
{
	class SHARP_API ITreeSolver : public ISolver
	{
	protected:
		ITreeSolver &operator=(ITreeSolver &) { return *this; };

	public:
		virtual ~ITreeSolver() = 0;

		virtual htd::ITreeDecomposition *decompose(
				const IDecomposableInstance &instance) const = 0;

		virtual ISolution *solve(
				const IDecomposableInstance &instance,
				const htd::ITreeDecomposition &decomposition) const = 0;

	}; // class ITreeSolver

	inline ITreeSolver::~ITreeSolver() { }
} // namespace sharp

#endif // SHARP_SHARP_ITREESOLVER_H_
