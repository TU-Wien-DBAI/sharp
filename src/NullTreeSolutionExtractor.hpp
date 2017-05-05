#ifndef SHARP_NULLTREESOLUTIONEXTRACTOR_H_
#define SHARP_NULLTREESOLUTIONEXTRACTOR_H_

#include <sharp/ITreeSolutionExtractor.hpp>

#include <cstddef>

namespace sharp
{
	class SHARP_LOCAL NullTreeSolutionExtractor : public ITreeSolutionExtractor
	{
	public:
		virtual ~NullTreeSolutionExtractor() override;

		virtual ISolution *extractSolution(
				htd::vertex_t node,
				const htd::ITreeDecomposition &decomposition,
				const INodeTableMap &tables,
				const IDecomposableInstance &instance) const override;

		virtual ISolution *emptySolution(
				const IDecomposableInstance &instance) const override;

	}; // class NullTreeSolutionExtractor

} // namespace sharp

#endif // SHARP_NULLTREESOLUTIONEXTRACTOR_H_
