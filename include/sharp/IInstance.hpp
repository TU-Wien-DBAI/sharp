#ifndef SHARP_SHARP_IINSTANCE_H_
#define SHARP_SHARP_IINSTANCE_H_

#include <sharp/global.hpp>

#include <htd/main>

namespace sharp
{
	class SHARP_API IInstance
	{
	protected:
		IInstance &operator=(IInstance &) { return *this; }

	public:
		virtual ~IInstance() = 0;

		virtual htd::IHypergraph *toHypergraph()
			const = 0;

	}; // class IInstance

	inline IInstance::~IInstance() { }
} // namespace sharp

#endif // SHARP_SHARP_IINSTANCE_H_
