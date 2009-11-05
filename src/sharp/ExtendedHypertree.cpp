
#include <cstdlib>
#include <string>
#include <algorithm>
#include <iostream>

#include "../Global.h"

#include "ExtendedHypertree.h"
#include "../htree/Node.h"

using namespace std;

ExtendedHypertree::ExtendedHypertree(Hypertree *node) : Hypertree()
{
	this->type = Unevaluated;
	this->difference = -1;
	
	this->MyParent = node->getParent();

	for(list<Hypertree *>::iterator i = node->getChildren()->begin(); i != node->getChildren()->end(); ++i)
		this->insChild(*i);

	node->getChildren()->clear();

	if(dynamic_cast<ExtendedHypertree *>(node) == NULL)
	{
		for(set<Node *>::iterator i = node->getChi()->begin(); i != node->getChi()->end(); ++i)
		{
			string name((*i)->getName());
			if(*name.begin() == 'r') this->rules.insert(atoi(name.substr(1).c_str()));
			else if(*name.begin() == 'v') this->variables.insert(atoi(name.substr(1).c_str()));
		}
	}
	else
	{
		this->rules = ((ExtendedHypertree *)node)->rules;
		this->variables = ((ExtendedHypertree *)node)->variables;
	}

	if(this->MyParent) { this->MyParent->remChild(node); this->MyParent->insChild(this); }

	delete node;
}

ExtendedHypertree::ExtendedHypertree(set<int> &rules, set<int> &variables) : Hypertree()
{
	this->variables = variables;
	this->rules = rules;
	this->type = Unevaluated;
	this->difference = -1;
}

ExtendedHypertree::~ExtendedHypertree() { }

ExtendedHypertree *ExtendedHypertree::createChild(ExtendedHypertree *child, set<int> rules, set<int> variables)
{
	ExtendedHypertree *parent = child->parent();
	ExtendedHypertree *newChild = new ExtendedHypertree(rules, variables);

	parent->remChild(child);
	newChild->insChild(child);
	parent->insChild(newChild);

	parent->type = parent->calculateType();

	return child;
}

TreeNodeType ExtendedHypertree::calculateType()
{
	if(this->MyChildren.size() == 0) return Leaf;
	if(this->MyChildren.size() == 2) return Branch;

	ExtendedHypertree *child = this->firstChild();
	vector<int> diff(1);

	if(child->variables.size() != this->variables.size() && containsAll(child->variables, this->variables))
	{
		set_difference(child->variables.begin(), child->variables.end(), this->variables.begin(), this->variables.end(), diff.begin());
		this->difference = diff[0];
		return VariableRemoval;
	}

	if(child->rules.size() != this->rules.size() && containsAll(child->rules, this->rules))
	{
		set_difference(child->rules.begin(), child->rules.end(), this->rules.begin(), this->rules.end(), diff.begin());
		this->difference = diff[0];
		return RuleRemoval;
	}
	
	if(this->variables.size() != child->variables.size() && containsAll(this->variables, child->variables))
	{
		set_difference(this->variables.begin(), this->variables.end(), child->variables.begin(), child->variables.end(), diff.begin());
		this->difference = diff[0];
		return VariableIntroduction;
	}

	if(this->rules.size() != child->rules.size() && containsAll(this->rules, child->rules))
	{
		set_difference(this->rules.begin(), this->rules.end(), child->rules.begin(), child->rules.end(), diff.begin());
		this->difference = diff[0];
		return RuleIntroduction;
	}

	return Unevaluated;
}

int ExtendedHypertree::getType() const
{
	return this->type;
}

int ExtendedHypertree::getDifference() const
{
	return this->difference;
}

bool ExtendedHypertree::isRoot() const
{
	return this->MyParent == NULL;
}

void ExtendedHypertree::normalize()
{	
	if(this->MyChildren.size() > 1)
	{
		ExtendedHypertree *l = new ExtendedHypertree(this->rules, this->variables);
		ExtendedHypertree *r = new ExtendedHypertree(this->rules, this->variables);
		
		l->insChild(*this->MyChildren.begin()); this->remChild(*this->MyChildren.begin());
		for(list<Hypertree *>::iterator i = this->MyChildren.begin(); i != this->MyChildren.end(); ++i)
			r->insChild(*i);

		this->getChildren()->clear();
		this->insChild(l);
		this->insChild(r);

		l->normalize();
		r->normalize();
	}
	else if(this->MyChildren.size() == 1)
	{
		ExtendedHypertree *child = dynamic_cast<ExtendedHypertree *>(*this->MyChildren.begin());
		if(child == NULL) child = new ExtendedHypertree(*this->MyChildren.begin());
		
		child->adapt();
		child->parent()->type = child->parent()->calculateType();
		child->normalize();
	}

	this->type = this->calculateType();
}

void ExtendedHypertree::adapt()
{
	if(this->MyParent == NULL) return;

	unsigned int changes;

	list<int> currentRules(this->parent()->rules.begin(), this->parent()->rules.end());
	list<int> currentVariables(this->parent()->variables.begin(), this->parent()->variables.end());

	vector<int> redRules(currentRules.size());
	vector<int> redVariables(currentVariables.size());
	vector<int> greenRules(this->rules.size());
	vector<int> greenVariables(this->variables.size());

	vector<int>::iterator it = set_difference(this->parent()->rules.begin(), this->parent()->rules.end(), 
							this->rules.begin(), this->rules.end(), redRules.begin());
	redRules.resize(it - redRules.begin());

	it = set_difference(this->parent()->variables.begin(), this->parent()->variables.end(),
				this->variables.begin(), this->variables.end(), redVariables.begin());
	redVariables.resize(it - redVariables.begin());

	it = set_difference(this->rules.begin(), this->rules.end(),
				this->parent()->rules.begin(), this->parent()->rules.end(), greenRules.begin());
	greenRules.resize(it - greenRules.begin());

	it = set_difference(this->variables.begin(), this->variables.end(),
				this->parent()->variables.begin(), this->parent()->variables.end(), greenVariables.begin());
	greenVariables.resize(it - greenVariables.begin());

	changes = redRules.size() + greenRules.size() + redVariables.size() + greenVariables.size();

	for(it = redRules.begin(); changes > 1 && it != redRules.end(); ++it)
	{
		currentRules.remove(*it);
		createChild(this, set<int>(currentRules.begin(), currentRules.end()), set<int>(currentVariables.begin(), currentVariables.end()));
		--changes;
	}

	for(it = redVariables.begin(); changes > 1 && it != redVariables.end(); ++it)
	{
		currentVariables.remove(*it);
		createChild(this, set<int>(currentRules.begin(), currentRules.end()), set<int>(currentVariables.begin(), currentVariables.end()));
		--changes;
	}

	for(it = greenRules.begin(); changes > 1 && it != greenRules.end(); ++it)
	{
		currentRules.push_back(*it);
		createChild(this, set<int>(currentRules.begin(), currentRules.end()), set<int>(currentVariables.begin(), currentVariables.end()));
		--changes;
	}
	
	for(it = greenVariables.begin(); changes > 1 && it != greenVariables.end(); ++it)
	{
		currentVariables.push_back(*it);
		createChild(this, set<int>(currentRules.begin(), currentRules.end()), set<int>(currentVariables.begin(), currentVariables.end()));
		--changes;
	}
}

ExtendedHypertree *ExtendedHypertree::parent() const
{
	return dynamic_cast<ExtendedHypertree *>(this->MyParent);
}

ExtendedHypertree *ExtendedHypertree::firstChild() const
{
	return dynamic_cast<ExtendedHypertree *>(*this->MyChildren.begin());
}

ExtendedHypertree *ExtendedHypertree::secondChild() const
{
	return dynamic_cast<ExtendedHypertree *>(*++this->MyChildren.begin());
}

const set<int> &ExtendedHypertree::getRules() const { return this->rules; }
const set<int> &ExtendedHypertree::getVariables() const { return this->variables; }
