#ifndef SHARP_HTDSOLUTIONTREE_H_
#define SHARP_HTDSOLUTIONTREE_H_

#include <libsharp/ISolution.hpp>
#include <libsharp/ITupleSolution.hpp>
#include <libsharp/ICumulativeSolutionBuilder.hpp>
#include <libsharp/IIterativeSolutionBuilder.hpp>

namespace sharp
{
	enum SolutionTreeOperation
	{
		LEAF,
		TRANSFORM,
		JOIN,
		MERGE
	};

	class HtdSolutionTree
	{
	public:
		HtdSolutionTree(ITupleSolution *leafSolution); // stores a leaf solution
		HtdSolutionTree(HtdSolutionTree *child, ITupleSolution *transformSolution); // stores a transform solution (one child)
		HtdSolutionTree(HtdSolutionTree *leftchild, HtdSolutionTree *rightchild, ITupleSolution *joinSolution); // stores a join solution (two children)
		HtdSolutionTree(HtdSolutionTree *merge1, HtdSolutionTree *merge2); // stores a merge of two solutions
		~HtdSolutionTree();

		bool hasLeftChild() const;
		bool hasRightChild() const;
		bool isRoot() const;
		bool hasParent() const;
		HtdSolutionTree &leftChild() const;
		HtdSolutionTree &rightChild() const;
		HtdSolutionTree &parent() const;
		ISolution *solution() const;

		void buildCumulativeSolution(const ICumulativeSolutionBuilder &builder);
		void buildIterativeSolution(const IIterativeSolutionBuilder &builder);

	private:
		void init();

		// owned memory
		ITupleSolution *m_localSolution;
		ISolution *m_solution;
		HtdSolutionTree *m_left;
		HtdSolutionTree *m_right;

		// external state information
		SolutionTreeOperation m_operation;
		HtdSolutionTree *m_parent;
		HtdSolutionTree *m_current;

		// internal state information
		mutable bool m_deleteSolution;

	}; // struct HtdSolutionTree

} // namespace sharp

#endif // SHARP_HTDSOLUTIONTREE_H_
