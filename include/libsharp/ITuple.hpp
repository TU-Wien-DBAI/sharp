#ifndef SHARP_LIBSHARP_ITUPLE_H_
#define SHARP_LIBSHARP_ITUPLE_H_

#include <libsharp/global.hpp>

#include <libsharp/IInstance.hpp>
#include <libsharp/ITupleSolution.hpp>

#include <htdecomp>

#include <functional>

namespace sharp
{
	using htd::vertex_t;
	using htd::ITreeDecomposition;

	class SHARP_API ITuple
	{

	protected:
		ITuple &operator=(const ITuple &) { return *this; }

	public:
		virtual ~ITuple() = 0;

		virtual ITupleSolution *extractTupleSolution(vertex_t node, const ITreeDecomposition &decomposition, const IInstance &instance) const = 0;
		virtual size_t hash() const = 0;
		virtual int compare(const ITuple &tuple) const = 0;

		bool operator<(const ITuple &tuple) const { return this->compare(tuple) < 0; }
		bool operator<=(const ITuple &tuple) const { return this->compare(tuple) <= 0; }
		bool operator>(const ITuple &tuple) const { return this->compare(tuple) > 0; }
		bool operator>=(const ITuple &tuple) const { return this->compare(tuple) >= 0; }
		bool operator==(const ITuple &tuple) const { return this->compare(tuple) == 0; }
	}; // class ITuple

	inline ITuple::~ITuple() { }
} // namespace sharp

#endif // SHARP_LIBSHARP_ITUPLE_H_
