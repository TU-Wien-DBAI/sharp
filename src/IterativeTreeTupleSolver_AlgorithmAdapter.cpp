#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#include "IterativeTreeTupleSolver.hpp"

#include "TupleSet.hpp"

namespace sharp
{
	using htd::vertex_t;
	using htd::ITreeDecomposition;
	using htd::ILabelingFunction;

	IterativeTreeTupleSolver::
	TupleToTreeAlgorithmAdapter::TupleToTreeAlgorithmAdapter(
			const ITreeTupleAlgorithm &algorithm)
		: algorithm_(algorithm)
	{ }

	IterativeTreeTupleSolver::
	TupleToTreeAlgorithmAdapter::~TupleToTreeAlgorithmAdapter() { }

	std::vector<const ILabelingFunction *>
	IterativeTreeTupleSolver::
	TupleToTreeAlgorithmAdapter::preprocessOperations() const
	{
		return algorithm_.preprocessOperations();
	}

	ITable *
	IterativeTreeTupleSolver::
	TupleToTreeAlgorithmAdapter::evaluateNode(
			vertex_t node,
			const ITreeDecomposition &decomposition,
			const INodeTableMap &tables,
			const IInstance &instance) const
	{
		TupleSet *newTable = new TupleSet();

		algorithm_.evaluateNode(
				node,
				decomposition,
				dynamic_cast<const INodeTupleSetMap &>(tables),
				instance,
				*newTable);

		return newTable;
	}

	bool
	IterativeTreeTupleSolver::
	TupleToTreeAlgorithmAdapter::needAllTables() const
	{
		return algorithm_.needAllTupleSets();
	}

} // namespace sharp
