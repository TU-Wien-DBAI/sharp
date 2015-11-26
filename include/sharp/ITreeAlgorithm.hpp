#ifndef SHARP_SHARP_ITREEALGORITHM_H_
#define SHARP_SHARP_ITREEALGORITHM_H_

#include <sharp/global>

#include <sharp/ISolution.hpp>
#include <sharp/ITable.hpp>
#include <sharp/INodeTableMap.hpp>

#include <htd/main>

#include <vector>

namespace sharp
{
	class SHARP_API ITreeAlgorithm
	{
	protected:
		ITreeAlgorithm &operator=(ITreeAlgorithm &) { return *this; };

	public:
		virtual ~ITreeAlgorithm() = 0;

		virtual std::vector<const htd::ILabelingFunction *>
			preprocessOperations() = 0;

		virtual ITable *evaluateNode(
				htd::vertex_t node,
				const htd::ITreeDecomposition &decomposition,
				const INodeTableMap &tables,
				const IInstance &instance) const = 0;

	}; // class ITreeAlgorithm

	inline ITreeAlgorithm::~ITreeAlgorithm() { }
} // namespace sharp

#endif // SHARP_SHARP_ITREEALGORITHM_H_
