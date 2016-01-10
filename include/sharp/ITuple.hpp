#ifndef SHARP_SHARP_ITUPLE_H_
#define SHARP_SHARP_ITUPLE_H_

#include <sharp/global>

#include <sharp/Enumerator.hpp>
#include <sharp/ConstEnumerator.hpp>

#include <cstddef>

namespace sharp
{
	class SHARP_API ITuple
	{
	protected:
		ITuple &operator=(ITuple &) { return *this; };

	public:
		virtual ~ITuple() = 0;

		virtual std::size_t hash() const = 0;
		virtual bool operator==(const ITuple &other) const = 0;

	}; // class ITuple

	inline ITuple::~ITuple() { }

} // namespace sharp

#endif // SHARP_SHARP_ITUPLE_H_
