#ifndef SHARP_LIBSHARP_IINSTANCE_H_
#define SHARP_LIBSHARP_IINSTANCE_H_

#include <libsharp/global.hpp>

#include <htdecomp>

namespace sharp
{
	using htd::IHypergraph;

	class SHARP_API IInstance
	{
	protected:
		IInstance &operator=(IInstance &) { return *this; }

	public:
		virtual ~IInstance() = 0;

		virtual IHypergraph *toHypergraphRepresentation() const = 0;

	}; // class IInstance

	inline IInstance::~IInstance() { }
} // namespace sharp

#endif // SHARP_LIBSHARP_IINSTANCE_H_
