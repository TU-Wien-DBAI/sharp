#ifndef HCFANSWERSET_H_
#define HCFANSWERSET_H_

#include <map>
#include <set>

#include "AnswerSet.h"

#ifndef DERIVED
#define DERIVED 0
#endif

typedef std::map<Vertex, VertexSet> Graph;

class HCFAnswerSetTuple : public Tuple
{
public:
	HCFAnswerSetTuple();
	HCFAnswerSetTuple(HCFAnswerSetTuple &o);
	virtual ~HCFAnswerSetTuple();

public:
	Graph graph; // one (partial) possible world
	Graph reversegraph; // the reverse graph
	RuleSet satisfied; // rules not in graph but satisfied

public:
	virtual bool operator<(const Tuple &other) const;
	virtual bool operator==(const Tuple &other) const;
	virtual int hash() const;

public:
	void removeVertex(Vertex v);
	void addVertex(Vertex v);
	void addEdge(Vertex v1, Vertex v2);
	void transitiveClosureThrough(Vertex v);
	bool hasCycleThrough(Vertex v);
};

class HCFAnswerSetAlgorithm : public AnswerSetAlgorithm
{
public:
	HCFAnswerSetAlgorithm(Problem *problem);
	virtual ~HCFAnswerSetAlgorithm();

protected:
	virtual Solution *selectSolution(TupleSet *tuples, const ExtendedHypertree *node);
	
	virtual TupleSet *evaluateLeafNode(const ExtendedHypertree *node);
	virtual TupleSet *evaluateBranchNode(const ExtendedHypertree *node);

	virtual TupleSet *evaluateVariableIntroductionNode(const ExtendedHypertree *node);
	virtual TupleSet *evaluateVariableRemovalNode(const ExtendedHypertree *node);
	virtual TupleSet *evaluateRuleIntroductionNode(const ExtendedHypertree *node);
	virtual TupleSet *evaluateRuleRemovalNode(const ExtendedHypertree *node);

public:
	HCFAnswerSetTuple *combineTuples(HCFAnswerSetTuple &l, HCFAnswerSetTuple &r, RuleSet rules);

	HCFAnswerSetTuple *copyTupleAddPositiveVariable(HCFAnswerSetTuple &x, RuleSet rules, Variable v);
	HCFAnswerSetTuple *copyTupleAddNegativeVariable(HCFAnswerSetTuple &x, RuleSet rules, Variable v);

	HCFAnswerSetTuple *copyTupleAddProofRule(HCFAnswerSetTuple &x, VariableSet variables, Rule r);
	HCFAnswerSetTuple *copyTupleAddNonProofRule(HCFAnswerSetTuple &x, VariableSet variables, Rule r);

	void addToTupleSet(Tuple &t, Solution *s, TupleSet *ts, Operation operation = Union);
};

#endif
