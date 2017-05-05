#ifndef SHARP_SHARP_IDECOMPOSABLEINSTANCE_H_
#define SHARP_SHARP_IDECOMPOSABLEINSTANCE_H_

#include <sharp/global>

#include <htd/main.hpp>

namespace sharp
{
	class SHARP_API IDecomposableInstance
	{
	protected:
		IDecomposableInstance &operator=(IDecomposableInstance &)
		{ return *this; }

	public:
		virtual ~IDecomposableInstance() = 0;

		virtual htd::IHypergraph *toHypergraph() const = 0;

	}; // class IDecomposableInstance

	inline IDecomposableInstance::~IDecomposableInstance() { }
} // namespace sharp

#endif // SHARP_SHARP_IDECOMPOSABLEINSTANCE_H_
