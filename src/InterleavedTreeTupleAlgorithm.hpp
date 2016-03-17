#ifndef SHARP_INTERLEAVEDTREETUPLEALGORITHM_H_
#define SHARP_INTERLEAVEDTREETUPLEALGORITHM_H_

#include <sharp/global>

#include <sharp/ITreeTupleAlgorithm.hpp>

namespace sharp
{
	class SHARP_LOCAL InterleavedTreeTupleAlgorithm : public ITreeTupleAlgorithm
	{
		const ITreeTupleAlgorithm &algorithm1_;
		const ITreeTupleAlgorithm &algorithm2_;

	public:
		InterleavedTreeTupleAlgorithm(
				const ITreeTupleAlgorithm &algorithm1,
				const ITreeTupleAlgorithm &algorithm2);

		virtual ~InterleavedTreeTupleAlgorithm() override;

		virtual std::vector<const htd::ILabelingFunction *>
			preprocessOperations() const override;

		virtual void evaluateNode(
				htd::vertex_t node,
				const htd::ITreeDecomposition &decomposition,
				const INodeTupleSetMap &tuples,
				const IInstance &instance,
				ITupleSet &outputTuples) const override;

		virtual bool needAllTupleSets() const override;
		
	}; // class InterleavedTreeTupleAlgorithm

} // namespace sharp

#endif // SHARP_INTERLEAVEDTREETUPLEALGORITHM_H_
