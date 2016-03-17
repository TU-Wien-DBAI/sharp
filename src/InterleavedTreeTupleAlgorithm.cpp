#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#include "InterleavedTreeTupleAlgorithm.hpp"

#include "NodeTupleSetMapOverlay.hpp"
#include "TupleSet.hpp"

#include <memory>

namespace sharp
{
	using htd::vertex_t;
	using htd::ITreeDecomposition;
	using htd::ILabelingFunction;

	using std::unique_ptr;
	using std::vector;

	InterleavedTreeTupleAlgorithm::InterleavedTreeTupleAlgorithm(
			const ITreeTupleAlgorithm &algorithm1,
			const ITreeTupleAlgorithm &algorithm2)
		: algorithm1_(algorithm1), algorithm2_(algorithm2)
	{ }

	InterleavedTreeTupleAlgorithm::~InterleavedTreeTupleAlgorithm() { }

	vector<const ILabelingFunction *>
	InterleavedTreeTupleAlgorithm::preprocessOperations() const
	{
		//TODO
		return vector<const ILabelingFunction *>();
	}

	void InterleavedTreeTupleAlgorithm::evaluateNode(
			vertex_t node,
			const ITreeDecomposition &decomposition,
			const INodeTupleSetMap &tuples,
			const IInstance &instance,
			ITupleSet &outputTuples) const
	{
		TupleSet intermediateOutputTuples;

		algorithm1_.evaluateNode(
				node,
				decomposition,
				tuples,
				instance,
				intermediateOutputTuples);

		NodeTupleSetMapOverlay overlay(tuples, node, intermediateOutputTuples);

		algorithm2_.evaluateNode(
				node,
				decomposition,
				overlay,
				instance,
				outputTuples);
	}

	bool InterleavedTreeTupleAlgorithm::needAllTupleSets() const
	{
		return algorithm1_.needAllTupleSets() || algorithm2_.needAllTupleSets();
	}

} // namespace sharp
