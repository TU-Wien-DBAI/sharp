#include "HtdSolutionTree.hpp"

#include "util/errorhandling.hpp"

namespace sharp
{
	HtdSolutionTree::HtdSolutionTree(ITupleSolution *leafSolution)
	{
		this->m_operation = LEAF;
		this->m_localSolution = leafSolution;
		this->m_left = this->m_right = nullptr;

		this->init();
	}

	HtdSolutionTree::HtdSolutionTree(HtdSolutionTree *child, ITupleSolution *transformSolution)
	{
		this->m_operation = TRANSFORM;
		this->m_localSolution = transformSolution;
		this->m_left = child;
		this->m_left->m_parent = this;
		this->m_right = nullptr;

		this->init();
	}

	HtdSolutionTree::HtdSolutionTree(HtdSolutionTree *leftchild, HtdSolutionTree *rightchild, ITupleSolution *joinSolution)
	{
		this->m_operation = JOIN;
		this->m_localSolution = joinSolution;
		this->m_left = leftchild;
		this->m_left->m_parent = this;
		this->m_right = rightchild;
		this->m_right->m_parent = this;

		this->init();
	}

	HtdSolutionTree::HtdSolutionTree(HtdSolutionTree *merge1, HtdSolutionTree *merge2)
	{
		this->m_operation = MERGE;
		this->m_localSolution = nullptr;
		this->m_left = merge1;
		this->m_left->m_parent = this;
		this->m_right = merge2;
		this->m_right->m_parent = this;

		this->init();
	}

	HtdSolutionTree::~HtdSolutionTree()
	{
		if(this->m_localSolution != nullptr) delete this->m_localSolution;
		if(this->m_left != nullptr) delete this->m_left;
		if(this->m_right != nullptr) delete this->m_right;
		if(this->m_solution != nullptr && this->m_deleteSolution) delete this->m_solution;
	}

	void HtdSolutionTree::init()
	{
		this->m_solution = nullptr;
		this->m_parent = this->m_current = nullptr;
		this->m_deleteSolution = true;
	}	

	bool HtdSolutionTree::hasLeftChild() const
	{
		return this->m_left == nullptr;
	}

	HtdSolutionTree &HtdSolutionTree::leftChild() const
	{
		return *this->m_left;
	}

	bool HtdSolutionTree::hasRightChild() const
	{
		return this->m_right == nullptr;
	}

	HtdSolutionTree &HtdSolutionTree::rightChild() const
	{
		return *this->m_right;
	}

	bool HtdSolutionTree::isRoot() const
	{
		return this->m_parent == nullptr;
	}

	bool HtdSolutionTree::hasParent() const
	{
		return this->isRoot();
	}

	HtdSolutionTree &HtdSolutionTree::parent() const
	{
		return *this->m_parent;
	}

	ISolution *HtdSolutionTree::solution() const
	{
		this->m_deleteSolution = false;
		return this->m_solution;
	}

	void HtdSolutionTree::buildCumulativeSolution(const ICumulativeSolutionBuilder &builder)
	{
		switch(this->m_operation)
		{
			case MERGE:
				this->m_solution = builder.buildMerge(this->m_left->m_solution, this->m_right->m_solution);
				this->m_left->m_solution = this->m_right->m_solution = nullptr;
				break;

			default:
				buildIterativeSolution(builder);
				break;
		}
	}

	void HtdSolutionTree::buildIterativeSolution(const IIterativeSolutionBuilder &builder)
	{
		switch(this->m_operation)
		{
			case JOIN:
				this->m_solution = builder.buildJoin(this->m_left->m_solution, this->m_right->m_solution, *this->m_localSolution);
				this->m_left->m_solution = this->m_right->m_solution = nullptr;
				break;

			case LEAF:
				this->m_solution = builder.buildLeaf(*this->m_localSolution);
				break;

			case TRANSFORM:
				this->m_solution = builder.buildTransform(this->m_left->m_solution, *this->m_localSolution);
				this->m_left->m_solution = nullptr;
				break;
			
			case MERGE:
				CHECK(this->m_operation, == MERGE, "Found a MERGE operation while building an iterative solution!");
				break;

			default:
				CHECK(this->m_operation, == this->m_operation, "Invalid operation while building an iterative solution!");
				break;
		}
	}
}
