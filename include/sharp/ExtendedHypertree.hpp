#ifndef EXTENDEDHYPERTREE_H_
#define EXTENDEDHYPERTREE_H_

#include <list>
#include <set>

#include <sharp/Global.hpp>

#include <sharp/Hypertree.hpp>

namespace sharp
{

	enum TreeNodeType
	{
		Unevaluated = 0,
		Leaf = 1, 
		Branch = 2, 
		Removal = 3, 
		Introduction = 4 
	};
		
	class ExtendedHypertree : public Hypertree
	{
	public:
		ExtendedHypertree(Hypertree *node);
		ExtendedHypertree(VertexSet &vertices);
		virtual ~ExtendedHypertree();
	
		void normalize();
	
		const VertexSet &getVertices() const;
		int getType() const;
		Vertex getDifference() const;
		bool isRoot() const;
	
		ExtendedHypertree *parent() const;
		ExtendedHypertree *firstChild() const;
		ExtendedHypertree *secondChild() const;
	
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

} // namespace sharp
#endif
