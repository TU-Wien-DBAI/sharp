#ifndef EXTENDEDHYPERTREE_H_
#define EXTENDEDHYPERTREE_H_

#include <list>
#include <set>

#include "../htree/Hypertree.h"

enum TreeNodeType
{
	Unevaluated = -1,
	Leaf = 1, 
	Branch = 2, 
	VariableRemoval = 3, 
	RuleRemoval = 4, 
	VariableIntroduction = 5, 
	RuleIntroduction = 6 
};
	
class ExtendedHypertree : public Hypertree
{
public:
	ExtendedHypertree(Hypertree *node);
	ExtendedHypertree(std::set<int> &rules, std::set<int> &variables);
	virtual ~ExtendedHypertree();
	void normalize();
	int getType() const;
	int getDifference() const;
	bool isRoot() const;
	const std::set<int> &getRules() const;
	const std::set<int> &getVariables() const;
	ExtendedHypertree *parent() const;
	ExtendedHypertree *firstChild() const;
	ExtendedHypertree *secondChild() const;

private:
	TreeNodeType type;
	int difference;
	
	std::set<int> rules;
	std::set<int> variables;

	void adapt();
	TreeNodeType calculateType();

	static ExtendedHypertree *createChild(ExtendedHypertree *child, std::set<int> rules, std::set<int> variables);
};
#endif
