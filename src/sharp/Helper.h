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

	static Partition partition(const std::set<Variable> &variables);
};

#endif
