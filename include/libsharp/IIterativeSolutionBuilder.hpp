#ifndef SHARP_LIBSHARP_IITERATIVESOLUTIONBUILDER_H_
#define SHARP_LIBSHARP_IITERATIVESOLUTIONBUILDER_H_

#include <libsharp/global.hpp>

#include <libsharp/ISolution.hpp>
#include <libsharp/ITupleSolution.hpp>

namespace sharp
{
	class SHARP_API IIterativeSolutionBuilder
	{
	protected:
		IIterativeSolutionBuilder &operator=(IIterativeSolutionBuilder &) { return *this; };

	public:
		virtual ~IIterativeSolutionBuilder() = 0;

		virtual ISolution *buildLeaf(const ITupleSolution &local) const = 0;
		virtual ISolution *buildTransform(ISolution *child, const ITupleSolution &local) const = 0;
		virtual ISolution *buildJoin(ISolution *leftChild, ISolution *rightChild, const ITupleSolution &local) const = 0;

	}; // class IIterativeSolutionBuilder

	inline IIterativeSolutionBuilder::~IIterativeSolutionBuilder() { }
} // namespace sharp

#endif // SHARP_LIBSHARP_IITERATIVESOLUTIONBUILDER_H_
