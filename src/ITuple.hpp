#ifndef SHARP_ITUPLE_H_
#define SHARP_ITUPLE_H_

#include <sharp/ITuple.hpp>

namespace std
{
	template<>
	struct SHARP_LOCAL hash<sharp::ITuple *>
	{
		size_t operator()(sharp::ITuple * const &tuple) const
		{
			return tuple->hash();
		}
	};

	template<>
	struct SHARP_LOCAL hash<const sharp::ITuple *>
	{
		size_t operator()(const sharp::ITuple * const &tuple) const
		{
			return tuple->hash();
		}
	};

	template<>
	struct SHARP_LOCAL equal_to<sharp::ITuple *>
	{
		bool operator()(
				sharp::ITuple * const &t1,
				sharp::ITuple * const &t2) const 
		{
			return *t1 == *t2;
		}
	};

	template<>
	struct SHARP_LOCAL equal_to<const sharp::ITuple *>
	{
		bool operator()(
				const sharp::ITuple * const &t1,
				const sharp::ITuple * const &t2) const 
		{
			return *t1 == *t2;
		}
	};

} // namespace std

#endif // SHARP_ITUPLE_H_
