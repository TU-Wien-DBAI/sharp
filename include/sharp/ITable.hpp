#ifndef SHARP_SHARP_ITABLE_H_
#define SHARP_SHARP_ITABLE_H_

#include <sharp/global>

namespace sharp
{
	class SHARP_API ITable
	{
	protected:
		ITable &operator=(ITable &) { return *this; };

	public:
		virtual ~ITable() = 0;

	}; // class ITable

	inline ITable::~ITable() { }
} // namespace sharp

#endif // SHARP_SHARP_ITABLE_H_
