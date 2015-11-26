#ifndef SHARP_ITUPLE_H_
#define SHARP_ITUPLE_H_

#include <sharp/ITuple.hpp>

namespace std
{
	template<>
	struct hash<sharp::ITuple>
	{
		size_t operator()(const ITuple &tuple)
		{
			return tuple.hash();
		}
	};

	template<>
	struct hash<sharp::ITuple *>
	{
		size_t operator()(const ITuple *&tuple)
		{
			return tuple->hash();
		}
	};

} // namespace std

#endif // SHARP_ITUPLE_H_
