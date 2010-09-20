#ifndef HEADCYCLEFREEANSWERSET_H_
#define HEADCYCLEFREEANSWERSET_H_

#include "AnswerSet.h"

class HeadCycleFreeAnswerSetTuple : public Tuple
{
public:
	HeadCycleFreeAnswerSetTuple();
	virtual ~HeadCycleFreeAnswerSetTuple();

public:
	set<Variable> variables;
	set<Rule> rules;

	Order order;
	OrderTypes ordertypes;

	map<Variable, set<Rule> > guards;
	set<set<Rule> > guardsdown;

public:
	virtual bool operator<(const Tuple &other) const;
	virtual bool operator==(const Tuple &other) const;
	virtual int hash() const;
};

class HeadCycleFreeAnswerSetAlgorithm : public AnswerSetAlgorithm
{
public:
	HeadCycleFreeAnswerSetAlgorithm(Problem *problem);
	virtual ~HeadCycleFreeAnswerSetAlgorithm();

protected:
	virtual Solution *selectSolution(TupleSet *tuples, const ExtendedHypertree *node);
	
	virtual TupleSet *evaluateLeafNode(const ExtendedHypertree *node);
	virtual TupleSet *evaluateBranchNode(const ExtendedHypertree *node);

	virtual TupleSet *evaluateVariableIntroductionNode(const ExtendedHypertree *node);
	virtual TupleSet *evaluateVariableRemovalNode(const ExtendedHypertree *node);
	virtual TupleSet *evaluateRuleIntroductionNode(const ExtendedHypertree *node);
	virtual TupleSet *evaluateRuleRemovalNode(const ExtendedHypertree *node);

public:
	static std::set<Rule> trueRules(const std::set<Variable> &positives,
					const std::set<Variable> &all,
					const std::set<Rule> &rules,
					Variable variable,
					const Order &order,
					const OrderTypes &ordertypes,
					const SignMap &signs,
					const HeadMap &heads);

	static std::set<Rule> trueRules(const std::set<Variable> &positives,
					const std::set<Variable> &all,
					const std::set<Rule> &rules,
					const Order &order,
					const OrderTypes &ordertypes,
					const SignMap &signs,
					const HeadMap &heads);

	static bool trueRule(	const std::set<Variable> &positives,
				const std::set<Variable> &all,
				Rule rule,
				Variable variable,
				const Order &order,
				const OrderTypes &ordertypes,
				const SignMap &signs,
				const HeadMap &heads);

	static OrderCombinations combineOrder(	const Order &original,
						const OrderTypes &types,
						const std::set<Rule> &left,
						const std::set<Rule> &right,
						int separator,
						bool separatorType);

	static OrderCombinations combineOrder(	const Order &original,
						const OrderTypes &types,
						int toInsert,
						bool insertType);

	static OrderCombinations combineOrder(	const Order &left,
						const OrderTypes &lefttypes,
						const Order &right,
						const OrderTypes &righttypes);
};

#endif
