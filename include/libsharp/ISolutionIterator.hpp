#ifndef SHARP_LIBSHARP_ISOLUTIONITERATOR_H_
#define SHARP_LIBSHARP_ISOLUTIONITERATOR_H_

#include <libsharp/global.hpp>

#include <libsharp/ISolution.hpp>

namespace sharp
{
	class SHARP_API ISolutionIterator
	{
	protected:
		ISolutionIterator &operator=(const ISolutionIterator &) { return *this; }

	public:
		virtual ~ISolutionIterator() = 0;

		virtual ISolution *next() = 0;

	}; // class ISolutionIterator

	inline ISolutionIterator::~ISolutionIterator() { }
} // namespace sharp

#endif // SHARP_LIBSHARP_ISOLUTIONITERATOR_H_
