#ifndef SHARP_SHARP_IHTDSOLVER_H_
#define SHARP_SHARP_IHTDSOLVER_H_

#include <sharp/global.hpp>

#include <sharp/IInstance.hpp>

namespace sharp
{
	class SHARP_API IHtdSolver
	{
	protected:
		IHtdSolver &operator=(IHtdSolver &) { return *this; };

	public:
		virtual ~IHtdSolver() = 0;

		virtual void solve(const IInstance &instance)
			const = 0;

	}; // class IHtdSolver

	inline IHtdSolver::~IHtdSolver() { }
} // namespace sharp

#endif // SHARP_SHARP_IHTDSOLVER_H_
