#ifndef SHARP_SHARP_ISOLVER_H_
#define SHARP_SHARP_ISOLVER_H_

#include <sharp/global>

#include <sharp/IInstance.hpp>
#include <sharp/ISolution.hpp>

namespace sharp
{
	class SHARP_API ISolver
	{
	protected:
		ISolver &operator=(ISolver &) { return *this; };

	public:
		virtual ~ISolver() = 0;

		virtual ISolution *solve(const IInstance &instance) const = 0;

	}; // class ISolver

	inline ISolver::~ISolver() { }
} // namespace sharp

#endif // SHARP_SHARP_ISOLVER_H_
