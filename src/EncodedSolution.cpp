#include "EncodedSolution.hpp"

#include <stack>

namespace
{ 
        using sharp::ISolution;
	using sharp::HtdSolutionTree;
	using sharp::ICumulativeSolutionBuilder;

	ISolution *buildCumulativeForTree(HtdSolutionTree &tree, const ICumulativeSolutionBuilder &builder)
	{		
		using std::stack;

		stack<HtdSolutionTree *> parents;
		const HtdSolutionTree *last = nullptr;
	       	HtdSolutionTree *current = &tree;

		while(!parents.empty() || current != nullptr)
		{
			if(current != nullptr)
			{
				parents.push(current);
				current = &current->leftChild();
				continue;
			}

			HtdSolutionTree *top = parents.top(), *topright;
			if(top->hasRightChild() && last != (topright = &top->rightChild()))
			{
				current = topright;
				continue;
			}

			current->buildCumulativeSolution(builder);
			last = current;
		}

		return tree.solution();
	}
}

namespace sharp
{
	EncodedSolution::EncodedSolution()
	{
		this->impl = new Impl();
	}

	EncodedSolution::~EncodedSolution()
	{
		for(auto tree : this->impl->trees)
			delete tree;

		delete this->impl;
	}

	ISolutionIterator *EncodedSolution::buildIterative(const IIterativeSolutionBuilder &builder) const
	{
		//TODO
		return nullptr;
	}

	ISolution *EncodedSolution::buildCumulative(const ICumulativeSolutionBuilder &builder) const
	{
		if(this->impl->trees.size() == 0) return builder.buildNone();

		auto treeIter = this->impl->trees.begin();
		ISolution *current = buildCumulativeForTree(**treeIter, builder);

		for(++treeIter; treeIter != this->impl->trees.end(); ++treeIter)
			current = builder.buildMerge(current, buildCumulativeForTree(**treeIter, builder));

		return current;
	}

} // namespace sharp
