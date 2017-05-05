#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#include "InterleavedTreeAlgorithm.hpp"

#include "NodeTableMapOverlay.hpp"

#include <memory>

namespace sharp
{
	using htd::vertex_t;
	using htd::ITreeDecomposition;
	using htd::ILabelingFunction;

	using std::unique_ptr;
	using std::vector;

	InterleavedTreeAlgorithm::InterleavedTreeAlgorithm(
			const ITreeAlgorithm &algorithm1,
			const ITreeAlgorithm &algorithm2)
		: algorithm1_(algorithm1), algorithm2_(algorithm2)
	{ }

	InterleavedTreeAlgorithm::~InterleavedTreeAlgorithm() { }

	vector<const ILabelingFunction *>
	InterleavedTreeAlgorithm::preprocessOperations() const
	{
		//TODO
		return vector<const ILabelingFunction *>();
	}

	ITable *InterleavedTreeAlgorithm::evaluateNode(
			vertex_t node,
			const ITreeDecomposition &decomposition,
			const INodeTableMap &tables,
			const IDecomposableInstance &instance) const
	{
		unique_ptr<ITable> table1(algorithm1_.evaluateNode(
					node,
					decomposition,
					tables,
					instance));

		NodeTableMapOverlay overlay(tables, node, *table1);

		return algorithm2_.evaluateNode(
				node,
				decomposition,
				overlay,
				instance);
	}

	bool InterleavedTreeAlgorithm::needAllTables() const
	{
		return algorithm1_.needAllTables() || algorithm2_.needAllTables();
	}

} // namespace sharp
