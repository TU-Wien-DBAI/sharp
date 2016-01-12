#ifdef HAVE_CONFIG_H
	#include <config.h>
#endif

#include "IterativeTreeTupleSolver.hpp"

#include "NullTreeTupleSolutionExtractor.hpp"
#include "NodeTupleSetMap.hpp"
#include "TupleSet.hpp"

#include <sharp/Benchmark.hpp>

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
	
	IterativeTreeTupleSolver::IterativeTreeTupleSolver(
			const ITreeDecompositionAlgorithm &decomposer,
			const ITreeTupleAlgorithm &algorithm)
		: impl(new Impl(decomposer, algorithm))
	{ }

	IterativeTreeTupleSolver::IterativeTreeTupleSolver(
			const ITreeDecompositionAlgorithm &decomposer,
			const ITreeTupleAlgorithm &algorithm,
			const ITreeTupleSolutionExtractor &extractor)
		: impl(new Impl(decomposer, algorithm, extractor))
	{ }

	IterativeTreeTupleSolver::~IterativeTreeTupleSolver()
	{
		delete this->impl;
	}

	size_t IterativeTreeTupleSolver::calculateTreewidth(
			const IInstance &instance) const
	{
		return this->impl->calculateTreewidth(instance);
	}

	ISolution *IterativeTreeTupleSolver::solve(const IInstance &instance) const
	{
		return this->impl->solve(instance);
	}

	IterativeTreeTupleSolver::Impl::Impl(
			const ITreeDecompositionAlgorithm &decomposer,
			const ITreeTupleAlgorithm &algorithm)
		: decomposer_(decomposer), algorithm_(algorithm)
	{
		extractor_ = new NullTreeTupleSolutionExtractor();
		deleteExtractor_ = true;
	}

	IterativeTreeTupleSolver::Impl::Impl(
			const ITreeDecompositionAlgorithm &decomposer,
			const ITreeTupleAlgorithm &algorithm,
			const ITreeTupleSolutionExtractor &extractor)
		: decomposer_(decomposer), algorithm_(algorithm)
	{
		extractor_ = &extractor;
		deleteExtractor_ = false;
	}

	IterativeTreeTupleSolver::Impl::~Impl()
	{
		if(deleteExtractor_) delete extractor_;
	}

	unique_ptr<ITreeDecomposition> IterativeTreeTupleSolver::Impl::decompose(
			const IInstance &instance) const
	{
		unique_ptr<IHypergraph> hg(instance.toHypergraph());
		Benchmark::registerTimestamp("hypergraph conversion time");

		//TODO: include preprocessingOperations in decomposition call
		//		once libhtd supports this
		return unique_ptr<ITreeDecomposition>(
				decomposer_.computeDecomposition(*hg));
	}

	unique_ptr<INodeTableMap> IterativeTreeTupleSolver::Impl::initializeMap(
			size_t decompositionNodeCount) const
	{
		return unique_ptr<INodeTableMap>(
				new NodeTupleSetMap(decompositionNodeCount));
	}

	void IterativeTreeTupleSolver::Impl::insertIntoMap(
			vertex_t node,
			const ITreeDecomposition &td,
			ITable *table,
			INodeTableMap &tables) const
	{
		IMutableNodeTupleSetMap &map =
			static_cast<IMutableNodeTupleSetMap &>(tables);

		map.insert(node, static_cast<ITupleSet *>(table));
		
		size_t childCount = td.childCount(node);
		for(size_t childIndex = 0; childIndex < childCount; ++childIndex)
			map.erase(td.child(node, childIndex));
	}

	ITable *IterativeTreeTupleSolver::Impl::evaluateNode(
			vertex_t node,
			const ITreeDecomposition &td,
			const INodeTableMap &tables,
			const IInstance &instance) const
	{
		ITupleSet *outputTuples = new TupleSet();

		algorithm_.evaluateNode(
				node,
				td,
				static_cast<const INodeTupleSetMap &>(tables),
				instance,
				*outputTuples);

		if(outputTuples->size() == 0)
		{
			delete outputTuples;
			return nullptr;
		}

		return outputTuples;
	}

	ISolution *IterativeTreeTupleSolver::Impl::extractSolution(
			vertex_t node,
			const ITreeDecomposition &td,
			const INodeTableMap &tables,
			const IInstance &instance) const
	{
		return extractor_->extractSolution(
				node,
				td, 
				static_cast<const INodeTupleSetMap &>(tables),
				instance);
	}

	ISolution *IterativeTreeTupleSolver::Impl::emptySolution(
			const IInstance &instance) const
	{
		return extractor_->emptySolution(instance);
	}

} // namespace sharp
