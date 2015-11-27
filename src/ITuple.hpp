#ifndef SHARP_ITUPLE_H_
#define SHARP_ITUPLE_H_

#include <sharp/ITuple.hpp>

namespace std
{
	template<>
	struct hash<sharp::ITuple *>
	{
		size_t operator()(sharp::ITuple * const &tuple) const
		{
			return tuple->hash();
		}
	};

	template<>
	struct hash<const sharp::ITuple *>
	{
		size_t operator()(const sharp::ITuple * const &tuple) const
		{
			return tuple->hash();
		}
	};
} // namespace std

#endif // SHARP_ITUPLE_H_
