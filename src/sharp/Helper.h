#ifndef HELPER_H_
#define HELPER_H_

#include <set>

#include "../Global.h"

class Helper
{
private:
	Helper();

public:
	static std::set<Rule> trueRules(const std::set<Variable> &positives, 
					const std::set<Variable> &negatives, 
					const std::set<Rule> &rules, 
					const SignMap &signs);

	static std::set<Rule> trueRules(const std::set<Variable> &base, 
					const std::set<Variable> &positives, 
					const std::set<Variable> &negatives, 
					const std::set<Rule> &rules, 
					const SignMap &signs,
					const HeadMap &heads);

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
				const SignMap &signs);

	static bool trueRule(	const std::set<Variable> &base,
				const std::set<Variable> &positives, 
				const std::set<Variable> &all, 
				Rule rule, 
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
				

	static Partition partition(const std::set<Variable> &variables);

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
