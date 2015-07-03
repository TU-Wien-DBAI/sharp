#ifndef SHARP_LIBSHARP_ISOLUTION_H_
#define SHARP_LIBSHARP_ISOLUTION_H_

#include <libsharp/global.hpp>

namespace sharp
{
	class SHARP_API ISolution
	{
	protected:
		ISolution &operator=(ISolution &) { return *this; };

	public:
		virtual ~ISolution() = 0;

	}; // class ISolution

	inline ISolution::~ISolution() { }
} // namespace sharp

#endif // SHARP_LIBSHARP_ISOLUTION_H_
