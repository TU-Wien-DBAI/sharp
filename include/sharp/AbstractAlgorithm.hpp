#ifndef ABSTRACTALGORITHM_H_
#define ABSTRACTALGORITHM_H_

//FIXME: use unordered_set/unordered_map when c++0x is released
#include <map>
#include <set>
#include <vector>

#include <sharp/Global.hpp>

#include <sharp/ExtendedHypertree.hpp>



namespace sharp
{
	class Problem;
	class Instantiator;
	
	enum Operation
	{
		Value,
		CrossJoin,
		Union,
		Add,
		NewLeaf,
		NewEmpty
	};
	
	class SolutionContent
	{
	public:
		SolutionContent();
		virtual ~SolutionContent() = 0;
	
		virtual SolutionContent *calculateUnion(SolutionContent *other) = 0;
		virtual SolutionContent *calculateCrossJoin(SolutionContent *other) = 0;
		virtual SolutionContent *calculateAdd(const VertexSet &toAdd) = 0;
	};
	
	class EnumerationSolutionContent : public SolutionContent
	{
	public:
		EnumerationSolutionContent();
		EnumerationSolutionContent(const VertexSet &partition);
		virtual ~EnumerationSolutionContent();
	
		virtual SolutionContent *calculateUnion(SolutionContent *other);
		virtual SolutionContent *calculateCrossJoin(SolutionContent *other);
		virtual SolutionContent *calculateAdd(const VertexSet &toAdd);
	
	public:
		std::set<VertexSet> enumerations;
	};
	
	class ConsistencySolutionContent : public SolutionContent
	{
	public:
		ConsistencySolutionContent();
		ConsistencySolutionContent(const VertexSet &partition);
		~ConsistencySolutionContent();
	
		virtual SolutionContent *calculateUnion(SolutionContent *other);
		virtual SolutionContent *calculateCrossJoin(SolutionContent *other);
		virtual SolutionContent *calculateAdd(const VertexSet &toAdd);
	
	public:
		bool consistent;
	};
	
	class Solution
	{
		friend class AbstractHTDAlgorithm;

	private:
		Solution(Operation operation, Solution *left, Solution *right);
		Solution(Solution *child, const VertexSet &toAdd);
		Solution(const VertexSet &partition);
		Solution(SolutionContent *content);
		Solution();
	
	public:
		~Solution();
	
		SolutionContent *getContent(Instantiator *instantiator = NULL);
	
	private:
		SolutionContent *content;
		Solution *leftArgument;
		Solution *rightArgument;
		VertexSet parameter;
		Operation operation;
	};
	
	class Tuple
	{
	public:
		Tuple();
		virtual ~Tuple();
	
	public:
		virtual bool operator<(const Tuple &other) const = 0;
		virtual bool operator==(const Tuple &other) const = 0;
		virtual int hash() const = 0;
	};
	
	//FIXME: use a hash_map, hash_function, etc...
	//typedef __gnu_cxx::hash_map<Tuple, Solution *> TupleSet;
	typedef std::map<Tuple *, Solution *, std::less<Tuple *> > TupleSet;
	
	class Instantiator
	{
	public:
		Instantiator();
		virtual ~Instantiator();
	
	public:
		virtual SolutionContent *createEmptySolution() const = 0;
		virtual SolutionContent *createLeafSolution(const VertexSet &partition) const = 0;
	};
	
	template<class TSolutionContent>
	class GenericInstantiator : public Instantiator
	{
	public:
		GenericInstantiator();
		virtual ~GenericInstantiator();
	
	public:
		virtual SolutionContent *createEmptySolution() const;
		virtual SolutionContent *createLeafSolution(const VertexSet &partition) const;
	};

	class AbstractHTDAlgorithm
	{
	public:
		AbstractHTDAlgorithm(Problem *problem);
		virtual ~AbstractHTDAlgorithm();
	
	private:
		Problem *prob;
		Instantiator *inst;

	public:
		Solution *evaluate(const ExtendedHypertree *root, Instantiator *instantiator = NULL);
	
	protected:
		virtual Problem *problem();
		virtual const ExtendedHypertree *prepareHypertreeDecomposition(const ExtendedHypertree *root);
		virtual Solution *selectSolution(TupleSet *tuples, const ExtendedHypertree *root) = 0;
		virtual TupleSet *evaluateNode(const ExtendedHypertree *node) = 0;

		Solution *createEmptySolution();
		Solution *createLeafSolution(const VertexSet &partition);
		Solution *combineSolutions(Operation operation, Solution *left, Solution *right);
		Solution *addToSolution(Solution *solution, const VertexSet &toAdd);
		Solution *addToSolution(Solution *solution, Vertex toAdd);
		void addToTupleSet(Tuple *t, Solution *s, TupleSet *ts, Operation mergeOperation = Union);
	};
	
	class AbstractSemiNormalizedHTDAlgorithm : public AbstractHTDAlgorithm
	{
	public:
		AbstractSemiNormalizedHTDAlgorithm(Problem *problem);
		virtual ~AbstractSemiNormalizedHTDAlgorithm();
	
	protected:
		virtual const ExtendedHypertree *prepareHypertreeDecomposition(const ExtendedHypertree *root);
		virtual TupleSet *evaluateNode(const ExtendedHypertree *node);	

		virtual TupleSet *evaluateBranchNode(const ExtendedHypertree *node) = 0;
		virtual TupleSet *evaluatePermutationNode(const ExtendedHypertree *node) = 0;
	};

	class AbstractNormalizedHTDAlgorithm : public AbstractSemiNormalizedHTDAlgorithm
	{
	public:
		AbstractNormalizedHTDAlgorithm(Problem *problem);
		virtual ~AbstractNormalizedHTDAlgorithm();
	
	protected:
		virtual const ExtendedHypertree *prepareHypertreeDecomposition(const ExtendedHypertree *root);

		virtual TupleSet *evaluatePermutationNode(const ExtendedHypertree *node);
		virtual TupleSet *evaluateIntroductionNode(const ExtendedHypertree *node) = 0;
		virtual TupleSet *evaluateRemovalNode(const ExtendedHypertree *node) = 0;
		virtual TupleSet *evaluateLeafNode(const ExtendedHypertree *node) = 0;
	};

	template<class TTuple>
	class AbstractStronglyNormalizedHTDAlgorithm: public AbstractNormalizedHTDAlgorithm
	{
	public:
		AbstractStronglyNormalizedHTDAlgorithm(Problem *problem);
		virtual ~AbstractStronglyNormalizedHTDAlgorithm();

	protected:
		virtual const ExtendedHypertree *prepareHypertreeDecomposition(const ExtendedHypertree *root);

		virtual Solution *selectSolution(TupleSet *tuples, const ExtendedHypertree *root);
		virtual TupleSet *evaluateLeafNode(const ExtendedHypertree *node);
	};
} // namespace sharp

namespace std
{
	template<>
	class less<sharp::Tuple *>
	{
	public:
		bool operator()(const sharp::Tuple *t1, const sharp::Tuple *t2) { return *t1 < *t2; }
	};

} // namespace std

namespace sharp
{
	/********************************************************\
	|* TEMPLATE: AbstractStronglyNormalizedHTDAlgorithm
	\********************************************************/
	template<class TTuple>
	AbstractStronglyNormalizedHTDAlgorithm<TTuple>::AbstractStronglyNormalizedHTDAlgorithm(Problem *problem)
		: AbstractNormalizedHTDAlgorithm(problem)
	{ }

	template<class TTuple>
	AbstractStronglyNormalizedHTDAlgorithm<TTuple>::~AbstractStronglyNormalizedHTDAlgorithm() { }

	template<class TTuple>
	const ExtendedHypertree *AbstractStronglyNormalizedHTDAlgorithm<TTuple>::prepareHypertreeDecomposition(const ExtendedHypertree *root)
	{
		return root->normalize(StrongNormalization);
	}

	template<class TTuple>
	Solution *AbstractStronglyNormalizedHTDAlgorithm<TTuple>::selectSolution(TupleSet *tuples, const ExtendedHypertree *root)
	{
		if(tuples->size() == (unsigned int)0) 
		{
			delete tuples;
			return createEmptySolution();
		}

		Solution *solution = tuples->begin()->second;
		delete tuples->begin()->first;
		delete tuples;
		return solution;
	}

	template<class TTuple>
	TupleSet *AbstractStronglyNormalizedHTDAlgorithm<TTuple>::evaluateLeafNode(const ExtendedHypertree *node)
	{
		TupleSet ts = new TupleSet();
		addToTupleSet(new TTuple(), createLeafSolution(VertexSet()), ts);
		return ts;
	}
	
	/***********************************\
	|* TEMPLATE: GenericInstantiator
	\***********************************/
	template<class TSolutionContent>
	GenericInstantiator<TSolutionContent>::GenericInstantiator() 
		: Instantiator()
	{
	}
	
	template<class TSolutionContent>
	GenericInstantiator<TSolutionContent>::~GenericInstantiator() { }

	template<class TSolutionContent>
	SolutionContent *GenericInstantiator<TSolutionContent>::createEmptySolution() const
	{
		return new TSolutionContent();
	}
	
	template<class TSolutionContent>
	SolutionContent *GenericInstantiator<TSolutionContent>::createLeafSolution(const VertexSet &partition) const
	{
		return new TSolutionContent(partition);
	}

} // namespace sharp

#endif /*ABSTRACTALGORITHM_H_*/
