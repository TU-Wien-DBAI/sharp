#ifndef SHARP_LIBSHARP_ITUPLESOLUTION_H_
#define SHARP_LIBSHARP_ITUPLESOLUTION_H_

#include <libsharp/global.hpp>

namespace sharp
{
	class SHARP_API ITupleSolution
	{
	protected:
		ITupleSolution &operator=(ITupleSolution &) { return *this; };

	public:
		virtual ~ITupleSolution() = 0;

	}; // class ITupleSolution

	inline ITupleSolution::~ITupleSolution() { }
} // namespace sharp

#endif // SHARP_LIBSHARP_ITUPLESOLUTION_H_
