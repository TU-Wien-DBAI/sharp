#ifndef SHARP_SHARP_ISOLUTION_H_
#define SHARP_SHARP_ISOLUTION_H_

#include <sharp/global>

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

#endif // SHARP_SHARP_ISOLUTION_H_
