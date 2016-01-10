#ifdef HAVE_CONFIG_H
	#include <config.h>
#endif

#include "IterativeTreeSolver.hpp"

#include "NullTreeSolutionExtractor.hpp"
#include "NodeTableMap.hpp"

#include <stack>
#include <memory>
#include <cstddef>

namespace sharp
{
	using htd::vertex_t;
	using htd::IHypergraph;
	using htd::ITreeDecomposition;
	using htd::ITreeDecompositionAlgorithm;

	using std::size_t;
	using std::unique_ptr;
	
	IterativeTreeSolver::IterativeTreeSolver(
			const ITreeDecompositionAlgorithm &decomposer,
			const ITreeAlgorithm &algorithm)
		: impl(new Impl(decomposer, algorithm))
	{ }

	IterativeTreeSolver::IterativeTreeSolver(
			const ITreeDecompositionAlgorithm &decomposer,
			const ITreeAlgorithm &algorithm,
			const ITreeSolutionExtractor &extractor)
		: impl(new Impl(decomposer, algorithm, extractor))
	{ }

	IterativeTreeSolver::~IterativeTreeSolver()
	{
		delete this->impl;
	}

	ISolution *IterativeTreeSolver::solve(const IInstance &instance) const
	{
		return this->impl->solve(instance);
	}

	IterativeTreeSolver::Impl::Impl(
			const ITreeDecompositionAlgorithm &decomposer,
			const ITreeAlgorithm &algorithm)
		: decomposer_(decomposer), algorithm_(algorithm)
	{
		extractor_ = new NullTreeSolutionExtractor();
		deleteExtractor_ = true;
	}

	IterativeTreeSolver::Impl::Impl(
			const ITreeDecompositionAlgorithm &decomposer,
			const ITreeAlgorithm &algorithm,
			const ITreeSolutionExtractor &extractor)
		: decomposer_(decomposer), algorithm_(algorithm)
	{
		extractor_ = &extractor;
		deleteExtractor_ = false;
	}

	IterativeTreeSolver::Impl::~Impl()
	{
		if(deleteExtractor_) delete extractor_;
	}

	unique_ptr<ITreeDecomposition> IterativeTreeSolver::Impl::decompose(
			const IInstance &instance) const
	{
		unique_ptr<IHypergraph> hg(instance.toHypergraph());
		//TODO: include preprocessingOperations in decomposition call
		//		once libhtd supports this
		return unique_ptr<ITreeDecomposition>(
				decomposer_.computeDecomposition(*hg));
	}

	unique_ptr<INodeTableMap> IterativeTreeSolver::Impl::initializeMap(
			size_t decompositionNodeCount) const
	{
		return unique_ptr<INodeTableMap>(
				new NodeTableMap(decompositionNodeCount));
	}

	void IterativeTreeSolver::Impl::insertIntoMap(
			vertex_t node,
			const ITreeDecomposition &td,
			ITable *table,
			INodeTableMap &tables) const
	{
		IMutableNodeTableMap &map =
			static_cast<IMutableNodeTableMap &>(tables);

		map.insert(node, table);
		
		size_t childCount = td.childCount(node);
		for(size_t childIndex = 0; childIndex < childCount; ++childIndex)
			map.erase(td.child(node, childIndex));
	}

	ITable *IterativeTreeSolver::Impl::evaluateNode(
			vertex_t node,
			const ITreeDecomposition &td,
			const INodeTableMap &tables,
			const IInstance &instance) const
	{
		return algorithm_.evaluateNode(node, td, tables, instance);
	}

	ISolution *IterativeTreeSolver::Impl::extractSolution(
			vertex_t node,
			const ITreeDecomposition &td,
			const INodeTableMap &tables,
			const IInstance &instance) const
	{
		return extractor_->extractSolution(node, td, tables, instance);
	}

	ISolution *IterativeTreeSolver::Impl::emptySolution(
			const IInstance &instance) const
	{
		return extractor_->emptySolution(instance);
	}

} // namespace sharp
