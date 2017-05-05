#ifndef SHARP_INTERLEAVEDTREEALGORITHM_H_
#define SHARP_INTERLEAVEDTREEALGORITHM_H_

#include <sharp/global>

#include <sharp/ITreeAlgorithm.hpp>

namespace sharp
{
	class SHARP_LOCAL InterleavedTreeAlgorithm : public ITreeAlgorithm
	{
		const ITreeAlgorithm &algorithm1_;
		const ITreeAlgorithm &algorithm2_;

	public:
		InterleavedTreeAlgorithm(
				const ITreeAlgorithm &algorithm1,
				const ITreeAlgorithm &algorithm2);

		virtual ~InterleavedTreeAlgorithm() override;

		virtual std::vector<const htd::ILabelingFunction *>
			preprocessOperations() const override;

		virtual ITable *evaluateNode(
				htd::vertex_t node,
				const htd::ITreeDecomposition &decomposition,
				const INodeTableMap &tables,
				const IDecomposableInstance &instance) const override;

		virtual bool needAllTables() const override;
		
	}; // class InterleavedTreeAlgorithm

} // namespace sharp

#endif // SHARP_INTERLEAVEDTREEALGORITHM_H_
