#ifndef SHARP_LIBSHARP_ITUPLEITERATOR_H_
#define SHARP_LIBSHARP_ITUPLEITERATOR_H_

#include <libsharp/global.hpp>

#include <libsharp/ITuple.hpp>

namespace sharp
{
	class SHARP_API ITupleIterator
	{
	protected:
		ITupleIterator &operator=(const ITupleIterator &) { return *this; }

	public:
		virtual ~ITupleIterator() = 0;

		virtual ITuple *next() = 0;

	}; // class ITupleIterator

	inline ITupleIterator::~ITupleIterator() { }
} // namespace sharp

#endif // SHARP_LIBSHARP_ITUPLEITERATOR_H_
