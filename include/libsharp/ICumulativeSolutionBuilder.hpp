#ifndef SHARP_LIBSHARP_ICUMULATIVESOLUTIONBUILDER_H_
#define SHARP_LIBSHARP_ICUMULATIVESOLUTIONBUILDER_H_

#include <libsharp/global.hpp>

#include <libsharp/IIterativeSolutionBuilder.hpp>
#include <libsharp/ISolution.hpp>
#include <libsharp/ITupleSolution.hpp>

namespace sharp
{
	class SHARP_API ICumulativeSolutionBuilder : public IIterativeSolutionBuilder
	{
	public:
		virtual ~ICumulativeSolutionBuilder() = 0;

		virtual ISolution *buildNone() const = 0;
		virtual ISolution *buildMerge(ISolution *leftChild, ISolution *rightChild) const = 0;

	}; // class ICumulativeSolutionBuilder

	inline ICumulativeSolutionBuilder::~ICumulativeSolutionBuilder() { }
} // namespace sharp

#endif // SHARP_LIBSHARP_ICUMULATIVESOLUTIONBUILDER_H_
