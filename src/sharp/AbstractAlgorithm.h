#ifndef ABSTRACTALGORITHM_H_
#define ABSTRACTALGORITHM_H_

//FIXME: use unordered_set/unordered_map when c++0x is released
#include <map>
#include <set>

#include <gmp.h>
#include <gmpxx.h>

#include "../Global.h"
#include "Helper.h"
#include "ExtendedHypertree.h"

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
	SolutionContent(const std::set<Variable> &partition);
	virtual ~SolutionContent() = 0;
};

class EnumerationSolutionContent : public SolutionContent
{
public:
	EnumerationSolutionContent();
	EnumerationSolutionContent(const std::set<Variable> &partition);
	virtual ~EnumerationSolutionContent();

public:
	std::set<std::set<Variable> > enumerations;
};

class CountingSolutionContent : public SolutionContent
{
public:
	CountingSolutionContent();
	CountingSolutionContent(const std::set<Variable> &partition);
	virtual ~CountingSolutionContent();

public:
	mpz_class count;
};

class ConsistencySolutionContent : public SolutionContent
{
public:
	ConsistencySolutionContent();
	ConsistencySolutionContent(const std::set<Variable> &partition);
	~ConsistencySolutionContent();

public:
	bool consistent;
};

class Solution
{
public:
	Solution(Operation operation, Solution *left, Solution *right);
	Solution(Solution *child, int difference);	
	Solution(const std::set<Variable> &partition);
	Solution();
	virtual ~Solution();

public:
	virtual SolutionContent *getContent();
	void forceCalculation();

protected:
	virtual void calculateUnion() = 0;
	virtual void calculateCrossJoin() = 0;
	virtual void calculateAddDifference() = 0;

protected:
	SolutionContent *content;
	Solution *leftArgument;
	Solution *rightArgument;
	int difference;

private:
	Operation operation;
};

class EnumerationSolution : public Solution
{
public:
	EnumerationSolution(Operation operation, Solution *left, Solution *right);
	EnumerationSolution(Solution *child, int difference);	
	EnumerationSolution(const std::set<Variable> &partition);
	EnumerationSolution();
	virtual ~EnumerationSolution();

protected:
	virtual void calculateUnion();
	virtual void calculateCrossJoin();
	virtual void calculateAddDifference();
};

class CountingSolution : public Solution
{
public:
	CountingSolution(Operation operation, Solution *left, Solution *right);
	CountingSolution(Solution *child, int difference);	
	CountingSolution(const std::set<Variable> &partition);
	CountingSolution();
	virtual ~CountingSolution();

protected:
	virtual void calculateUnion();
	virtual void calculateCrossJoin();
	virtual void calculateAddDifference();
};

class ConsistencySolution : public Solution
{
public:
	ConsistencySolution(Operation operation, Solution *left, Solution *right);
	ConsistencySolution(Solution *child, int difference);	
	ConsistencySolution(const std::set<Variable> &partition);
	ConsistencySolution();
	virtual ~ConsistencySolution();

protected:
	virtual void calculateUnion();
	virtual void calculateCrossJoin();
	virtual void calculateAddDifference();
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
	Instantiator();
	virtual ~Instantiator();

public:
	virtual Solution *createEmptySolution() const = 0;
	virtual Solution *createLeafSolution(const std::set<Variable> &partition) const = 0;
	virtual Solution *combine(Operation operation, Solution *left, Solution *right) const = 0;
	virtual Solution *addDifference(Solution *child, int difference) const = 0;
};

template<class TSolution>
class GenericInstantiator : public Instantiator
{
public:
	GenericInstantiator(bool lazy);
	virtual ~GenericInstantiator();

public:
	virtual TSolution *createEmptySolution() const;
	virtual TSolution *createLeafSolution(const std::set<Variable> &partition) const;
	virtual TSolution *combine(Operation operation, Solution *left, Solution *right) const;
	virtual TSolution *addDifference(Solution *child, int difference) const;

private:
	bool lazy;
};

class AbstractAlgorithm
{
public:
	AbstractAlgorithm(const Instantiator *instantiator, const ExtendedHypertree *root, 
		const SignMap &signMap, const HeadMap &headMap, const NameMap &nameMap);
	virtual ~AbstractAlgorithm();

protected:
	const Instantiator *instantiator;
	const ExtendedHypertree *root;
	SignMap signMap;
	HeadMap headMap;
	NameMap nameMap;

public:
	virtual Solution *evaluate();
	SignMap &getSignMap();
	HeadMap &getHeadMap();
	NameMap &getNameMap();

protected:
	virtual Solution *selectSolution(TupleSet *tuples) = 0;

	virtual TupleSet *evaluateLeafNode(const ExtendedHypertree *node) = 0;
	virtual TupleSet *evaluateBranchNode(const ExtendedHypertree *node) = 0;
	virtual TupleSet *evaluateVariableIntroductionNode(const ExtendedHypertree *node) = 0;
	virtual TupleSet *evaluateVariableRemovalNode(const ExtendedHypertree *node) = 0;
	virtual TupleSet *evaluateRuleIntroductionNode(const ExtendedHypertree *node) = 0;
	virtual TupleSet *evaluateRuleRemovalNode(const ExtendedHypertree *node) = 0;

	TupleSet *evaluateNode(const ExtendedHypertree *node);	
};

#include "AbstractAlgorithmTemplates.cpp"

#endif /*ABSTRACTALGORITHM_H_*/
