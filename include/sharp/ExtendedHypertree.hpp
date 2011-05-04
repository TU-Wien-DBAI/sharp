#ifndef EXTENDEDHYPERTREE_H_
#define EXTENDEDHYPERTREE_H_

#include <sharp/Global.hpp>
#include <sharp/Hypertree.hpp>

namespace sharp
{
	enum TreeNodeType
	{
		General = 0,
		Leaf = 1, 
		Branch = 2, 
		Removal = 3, 
		Introduction = 4,
		Permutation = 5
	};

	enum NormalizationType
	{
		DefaultNormalization,
		SemiNormalization,
		StrongNormalization,
		NoNormalization
	};
		
	class ExtendedHypertree : public Hypertree
	{
	public:
		ExtendedHypertree(Hypertree *node);
		ExtendedHypertree(const VertexSet &vertices);
		virtual ~ExtendedHypertree();

		ExtendedHypertree *normalize(NormalizationType type = DefaultNormalization);

		const VertexSet &getVertices() const;
		int getType() const;
		Vertex getDifference() const;
		bool isRoot() const;

		ExtendedHypertree *parent() const;
		ExtendedHypertree *firstChild() const;
		ExtendedHypertree *secondChild() const;

		// Returns the average number of elements in the chi-set over all
		// nodes in the subtree
		double getAverageWidth();

		// Gets the sum of all elements in the chi-sets over all nodes in
		// the subtree
		unsigned int getWidthSum();

		unsigned int getJoinNodeWidthSum();

		double getAverageJoinNodeWidth();

		unsigned int getNodeCountWithWidth(unsigned int width);

		double getAverageJoinLeafDistance();

		double getAverageJoinJoinDistance();

		double getJoinNodePercentage();

		unsigned int getDistanceToNearestLeaf();
		unsigned int getDistanceToNearestJoinAncestor();
		unsigned int getJoinLeafDistanceSum();
		unsigned int getJoinJoinDistanceSum();
		virtual unsigned int getNumberOfJoinNodes();

	private:
		TreeNodeType type;
		Vertex difference;

		VertexSet vertices;
		
		void adapt();
		TreeNodeType calculateType();

		static ExtendedHypertree *createChild(ExtendedHypertree *child, VertexSet vertices);

#ifdef DEBUG
	public: //PRINTING FUNCTIONS
		void print();
#endif
	};
}

#endif
