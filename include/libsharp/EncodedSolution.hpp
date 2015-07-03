#ifndef SHARP_LIBSHARP_ENCODEDSOLUTION_H_
#define SHARP_LIBSHARP_ENCODEDSOLUTION_H_

#include <libsharp/global.hpp>

#include <libsharp/IIterativeSolutionBuilder.hpp>
#include <libsharp/ISolutionIterator.hpp>
#include <libsharp/ICumulativeSolutionBuilder.hpp>
#include <libsharp/ISolution.hpp>

namespace sharp
{
	class HtdSolver;

	class SHARP_API EncodedSolution
	{
	protected:
		EncodedSolution &operator=(const EncodedSolution &) { return *this; }

	public:
		~EncodedSolution();

		ISolutionIterator *buildIterative(const IIterativeSolutionBuilder &builder) const;
		ISolution *buildCumulative(const ICumulativeSolutionBuilder &builder) const;

	private:
		struct Impl;
		Impl *impl;

		SHARP_LOCAL EncodedSolution();

		friend class HtdSolver;
	}; // class EncodedSolution

} // namespace sharp

#endif // SHARP_LIBSHARP_ENCODEDSOLUTION_H_
