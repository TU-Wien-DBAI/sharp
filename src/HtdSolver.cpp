#include "HtdSolver.hpp"

#include "HtdSolutionTreeBuilder.hpp"
#include "EncodedSolution.hpp"

namespace sharp
{
	HtdSolver::Impl::Impl(const IHtdAlgorithm &algorithm)
		: algorithm(algorithm)
	{ }

	HtdSolver::HtdSolver(const IHtdAlgorithm &algorithm)
	{
		this->impl = new Impl(algorithm);
	}

	HtdSolver::~HtdSolver()
	{
		delete this->impl;
	}

	ISolutionIterator *HtdSolver::computeSolution(const IInstance &instance, const ITreeDecomposition &decomposition, const IIterativeSolutionBuilder &builder) const
	{
		//TODO
		return nullptr;
	}

	EncodedSolution *HtdSolver::computeEncodedSolution(const IInstance &instance, const ITreeDecomposition &decomposition) const
	{
		auto *encsol = new EncodedSolution();
		HtdSolutionTreeBuilder treebuilder(this->impl->algorithm, instance, decomposition);
		encsol->impl->trees = treebuilder.buildTrees();
		return encsol;
	}

} // namespace sharp
