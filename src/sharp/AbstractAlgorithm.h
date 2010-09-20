#ifndef ABSTRACTALGORITHM_H_
#define ABSTRACTALGORITHM_H_

//FIXME: use unordered_set/unordered_map when c++0x is released
#include <map>
#include <set>

#include <gmp.h>
#include <gmpxx.h>

#include "../Global.h"
#include "Helper.h"
#include "../htree/ExtendedHypertree.h"

enum Operation
{
	Value,
	CrossJoin,
	Union,
	AddDifference	
};

class SolutionContent
{
public:
	SolutionContent();
	SolutionContent(const VertexSet &partition);
	virtual ~SolutionContent() = 0;

	virtual SolutionContent *calculateUnion(SolutionContent *other) = 0;
	virtual SolutionContent *calculateCrossJoin(SolutionContent *other) = 0;
	virtual SolutionContent *calculateAddDifference(Vertex difference) = 0;
};

class EnumerationSolutionContent : public SolutionContent
{
public:
	EnumerationSolutionContent();
	EnumerationSolutionContent(const VertexSet &partition);
	virtual ~EnumerationSolutionContent();

	virtual SolutionContent *calculateUnion(SolutionContent *other);
	virtual SolutionContent *calculateCrossJoin(SolutionContent *other);
	virtual SolutionContent *calculateAddDifference(Vertex difference);

public:
	std::set<VertexSet> enumerations;
};

class CountingSolutionContent : public SolutionContent
{
public:
	CountingSolutionContent();
	CountingSolutionContent(const VertexSet &partition);
	virtual ~CountingSolutionContent();

	virtual SolutionContent *calculateUnion(SolutionContent *other);
	virtual SolutionContent *calculateCrossJoin(SolutionContent *other);
	virtual SolutionContent *calculateAddDifference(Vertex difference);

public:
	mpz_class count;
};

class ConsistencySolutionContent : public SolutionContent
{
public:
	ConsistencySolutionContent();
	ConsistencySolutionContent(const VertexSet &partition);
	~ConsistencySolutionContent();

	virtual SolutionContent *calculateUnion(SolutionContent *other);
	virtual SolutionContent *calculateCrossJoin(SolutionContent *other);
	virtual SolutionContent *calculateAddDifference(Vertex difference);

public:
	bool consistent;
};

class Solution
{
public:
	Solution(Operation operation, Solution *left, Solution *right);
	Solution(Solution *child, int difference);	
	Solution(SolutionContent *content);
	~Solution();

public:
	SolutionContent *getContent();
	void forceCalculation();

protected:
	void calculateUnion();
	void calculateCrossJoin();
	void calculateAddDifference();

protected:
	SolutionContent *content;
	Solution *leftArgument;
	Solution *rightArgument;
	int difference;

private:
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

namespace std
{
	template<>
	class less<Tuple *>
	{
	public:
		bool operator()(const Tuple *t1, const Tuple *t2) { return *t1 < *t2; }
	};
}

//FIXME: use a hash_map, hash_function, etc...
//typedef __gnu_cxx::hash_map<Tuple, Solution *> TupleSet;
typedef std::map<Tuple *, Solution *, less<Tuple *> > TupleSet;

class Instantiator
{
public:
	Instantiator(bool lazy);
	virtual ~Instantiator();

public:
	virtual Solution *createEmptySolution() const = 0;
	virtual Solution *createLeafSolution(const VertexSet &partition) const = 0;
	virtual Solution *combine(Operation operation, Solution *left, Solution *right) const;
	virtual Solution *addDifference(Solution *child, int difference) const;

protected:
	bool lazy;
};

template<class TSolutionContent>
class GenericInstantiator : public Instantiator
{
public:
	GenericInstantiator(bool lazy);
	virtual ~GenericInstantiator();

public:
	virtual Solution *createEmptySolution() const;
	virtual Solution *createLeafSolution(const VertexSet &partition) const;
};

class Problem;

class AbstractAlgorithm
{
public:
	AbstractAlgorithm(Problem *problem);
	virtual ~AbstractAlgorithm();

protected:
	Instantiator *instantiator;
	Problem *problem;

public:
	void setInstantiator(Instantiator *instantiator);
	Solution *evaluate(const ExtendedHypertree *root);

protected:
	virtual Solution *selectSolution(TupleSet *tuples, const ExtendedHypertree *root) = 0;

	virtual TupleSet *evaluateLeafNode(const ExtendedHypertree *node) = 0;
	virtual TupleSet *evaluateBranchNode(const ExtendedHypertree *node) = 0;
	virtual TupleSet *evaluateIntroductionNode(const ExtendedHypertree *node) = 0;
	virtual TupleSet *evaluateRemovalNode(const ExtendedHypertree *node) = 0;

	TupleSet *evaluateNode(const ExtendedHypertree *node);	
};

#include "AbstractAlgorithmTemplates.cpp"

#endif /*ABSTRACTALGORITHM_H_*/
