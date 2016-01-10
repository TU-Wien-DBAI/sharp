#ifndef SHARP_SHARP_HASHER_H_
#define SHARP_SHARP_HASHER_H_

#include <sharp/global>

#include <cstddef>

namespace sharp
{
	template<typename T>
	struct SHARP_API Hasher
	{
		std::size_t operator()(const T &object) const
		{
			return object.hash();
		}
	};

} // namespace sharp

#endif // SHARP_SHARP_HASHER_H_
