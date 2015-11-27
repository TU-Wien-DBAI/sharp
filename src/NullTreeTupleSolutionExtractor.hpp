#ifndef SHARP_NULLTREETUPLESOLUTIONEXTRACTOR_H_
#define SHARP_NULLTREETUPLESOLUTIONEXTRACTOR_H_

#include <sharp/ITreeTupleSolutionExtractor.hpp>

#include <cstddef>

namespace sharp
{
	class SHARP_LOCAL NullTreeTupleSolutionExtractor
		: public ITreeTupleSolutionExtractor
	{
	public:
		virtual ~NullTreeTupleSolutionExtractor() override;

		virtual ISolution *extractSolution(
				htd::vertex_t node,
				const htd::ITreeDecomposition &decomposition,
				const INodeTupleSetMap &tuples,
				const IInstance &instance) const override;

		virtual ISolution *emptySolution(
				const IInstance &instance) const override;

	}; // class NullTreeTupleSolutionExtractor

} // namespace sharp

#endif // SHARP_NULLTREETUPLESOLUTIONEXTRACTOR_H_
