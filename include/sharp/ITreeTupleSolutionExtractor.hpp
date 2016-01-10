#ifndef SHARP_SHARP_ITREETUPLESOLUTIONEXTRACTOR_H_
#define SHARP_SHARP_ITREETUPLESOLUTIONEXTRACTOR_H_

#include <sharp/global>

#include <sharp/ISolution.hpp>
#include <sharp/ITupleSet.hpp>
#include <sharp/INodeTupleSetMap.hpp>
#include <sharp/IInstance.hpp>

#include <htd/main.hpp>

#include <vector>

namespace sharp
{
	class SHARP_API ITreeTupleSolutionExtractor
	{
	protected:
		ITreeTupleSolutionExtractor &operator=(ITreeTupleSolutionExtractor &)
		{ return *this; };

	public:
		virtual ~ITreeTupleSolutionExtractor() = 0;

		virtual ISolution *extractSolution(
				htd::vertex_t node,
				const htd::ITreeDecomposition &decomposition,
				const INodeTupleSetMap &tuples,
				const IInstance &instance) const = 0;

		virtual ISolution *emptySolution(const IInstance &instance) const = 0;

	}; // class ITreeTupleSolutionExtractor

	inline ITreeTupleSolutionExtractor::~ITreeTupleSolutionExtractor() { }
} // namespace sharp

#endif // SHARP_SHARP_ITREETUPLESOLUTIONEXTRACTOR_H_
