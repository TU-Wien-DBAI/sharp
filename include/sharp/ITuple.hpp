#ifndef SHARP_SHARP_ITUPLE_H_
#define SHARP_SHARP_ITUPLE_H_

#include <sharp/global>

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
		virtual bool operator==(const ITuple &tuple) const = 0;
		virtual bool operator<(const ITuple &tuple) const = 0;

	}; // class ITuple

	inline ITuple::~ITuple() { }
} // namespace sharp

namespace std
{
	template<>
	struct less<sharp::ITuple *>
	{
		bool operator()(const sharp::ITuple *&t1, const sharp::ITuple *&t2)
		{
			return *t1 < *t2;
		}
	};

	template<>
	struct equal_to<sharp::ITuple *>
	{
		bool operator()(const sharp::ITuple *&t1, const sharp::ITuple *&t2)
		{
			return *t1 == *t2;
		}
	};
} // namespace std

#endif // SHARP_SHARP_ITUPLE_H_
