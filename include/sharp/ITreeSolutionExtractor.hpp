#ifndef SHARP_SHARP_ITREESOLUTIONEXTRACTOR_H_
#define SHARP_SHARP_ITREESOLUTIONEXTRACTOR_H_

#include <sharp/global>

#include <sharp/ISolution.hpp>
#include <sharp/ITable.hpp>
#include <sharp/INodeTableMap.hpp>
#include <sharp/IDecomposableInstance.hpp>

#include <htd/main.hpp>

namespace sharp
{
	class SHARP_API ITreeSolutionExtractor
	{
	protected:
		ITreeSolutionExtractor &operator=(ITreeSolutionExtractor &)
		{ return *this; };

	public:
		virtual ~ITreeSolutionExtractor() = 0;

		virtual ISolution *extractSolution(
				htd::vertex_t node,
				const htd::ITreeDecomposition &decomposition,
				const INodeTableMap &tables,
				const IDecomposableInstance &instance) const = 0;

		virtual ISolution *emptySolution(
				const IDecomposableInstance &instance) const = 0;

	}; // class ITreeSolutionExtractor

	inline ITreeSolutionExtractor::~ITreeSolutionExtractor() { }
} // namespace sharp

#endif // SHARP_SHARP_ITREESOLUTIONEXTRACTOR_H_
