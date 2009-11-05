#ifndef ABSTRACTALGORITHM_H_
#define ABSTRACTALGORITHM_H_

//FIXME: use unordered_set when c++0x is released
//#include <ext/hash_map>
#include <map>
#include <set>

#include "../Global.h"
#include "Helper.h"
#include "ExtendedHypertree.h"

enum Operation
{
	Value,
	LazyCrossJoin,
	LazyUnion,
	LazyAddDifference	
};

class SolutionContent
{
public:
	SolutionContent();
	virtual ~SolutionContent() = 0;
};

class Solution
{
public:
	Solution();
	Solution(SolutionContent *content);
	virtual ~Solution();

public:
	virtual SolutionContent *getContent();

protected:
	SolutionContent *content;
};

class LazySolution : public Solution
{
public:
	LazySolution(Operation operation, Solution *left, Solution *right);
	LazySolution(Solution *child, int difference);	
	virtual ~LazySolution();

public:
	virtual SolutionContent *getContent();

private:
	inline void calculate();

protected:
	virtual void calculateUnion() = 0;
	virtual void calculateCrossJoin() = 0;
	virtual void calculateAddDifference() = 0;

private:
	Operation operation;

protected:
	Solution *leftArgument;
	Solution *rightArgument;
	int difference;
};

class Tuple
{
public:
	Tuple();
	virtual ~Tuple() = 0;

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
	virtual ~Instantiator() = 0;

public:
	virtual Solution *createEmptySolution() const = 0;
	virtual Solution *createLeafSolution(const std::set<Variable> &partition) const = 0;
	virtual Solution *combine(Operation operation, Solution *left, Solution *right) const = 0;
	virtual Solution *addDifference(Solution *child, int difference) const = 0;
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
	Solution *evaluate();
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

protected:
	TupleSet *evaluateNode(const ExtendedHypertree *node);	
};

#endif /*ABSTRACTALGORITHM_H_*/
