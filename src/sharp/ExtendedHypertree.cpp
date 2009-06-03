
#include <cstdlib>
#include <string>
#include <algorithm>

#include <iostream>

using namespace std;

#include "../Global.h"

#include "ExtendedHypertree.h"
#include "../htree/Node.h"

ExtendedHypertree::ExtendedHypertree(Hypertree *node) : Hypertree()
{
	this->MyParent = node->getParent();

	for(list<Hypertree *>::iterator i = node->getChildren()->begin(); i != node->getChildren()->end(); ++i)
		this->insChild(*i);

	node->getChildren()->clear();

	if(dynamic_cast<ExtendedHypertree *>(node) == NULL)
	{
		for(set<Node *>::iterator i = node->getChi()->begin(); i != node->getChi()->end(); ++i)
		{
			string name((*i)->getName());
			if(*name.begin() == 'c') this->clauses.insert(atoi(name.substr(1).c_str()));
			else if(*name.begin() == 'v') this->variables.insert(atoi(name.substr(1).c_str()));
		}
	}
	else
	{
		this->clauses = ((ExtendedHypertree *)node)->clauses;
		this->variables = ((ExtendedHypertree *)node)->variables;
	}

	if(this->MyParent) { this->MyParent->remChild(node); this->MyParent->insChild(this); }

	delete node;
}

ExtendedHypertree::ExtendedHypertree(set<int> clauses, set<int> variables) : Hypertree()
{
	this->variables = variables;
	this->clauses = clauses;
}

ExtendedHypertree::~ExtendedHypertree() { }

Hypertree *ExtendedHypertree::createChild(Hypertree *child, set<int> clauses, set<int> variables)
{
	Hypertree *parent = child->getParent();
	Hypertree *newChild = new ExtendedHypertree(clauses, variables);

	parent->remChild(child);
	newChild->insChild(child);
	parent->insChild(newChild);

	return child;
}

int ExtendedHypertree::getType() const
{
	if(this->MyChildren.size() == 0) return ExtendedHypertree::LEAF;
	if(this->MyChildren.size() == 2) return ExtendedHypertree::BRANCH;

	Hypertree *c = *this->MyChildren.begin();
	ExtendedHypertree *child = dynamic_cast<ExtendedHypertree *>(c);

	if(child->variables.size() != this->variables.size() && containsAll(child->variables, this->variables)) return ExtendedHypertree::VARREM;
	if(child->clauses.size() != this->clauses.size() && containsAll(child->clauses, this->clauses)) return ExtendedHypertree::CLREM;
	if(this->variables.size() != child->variables.size() && containsAll(this->variables, child->variables)) return ExtendedHypertree::VARINTR;
	if(this->clauses.size() != child->clauses.size() && containsAll(this->clauses, child->clauses)) return ExtendedHypertree::CLINTR;

	return -1;
}

bool ExtendedHypertree::isRoot() const
{
	return this->MyParent == NULL;
}

void ExtendedHypertree::normalize()
{	
	if(this->MyChildren.size() > 1)
	{
		ExtendedHypertree *l = new ExtendedHypertree(this->clauses, this->variables);
		ExtendedHypertree *r = new ExtendedHypertree(this->clauses, this->variables);
		
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
		child->normalize();
	}
	else
	{
		new ExtendedHypertree(this);
	}
}

void ExtendedHypertree::adapt()
{
	if(this->MyParent == NULL) return;

	unsigned int changes;

	list<int> currentClauses(this->parent()->clauses.begin(), this->parent()->clauses.end());
	list<int> currentVariables(this->parent()->variables.begin(), this->parent()->variables.end());

	vector<int> redClauses(currentClauses.size());
	vector<int> redVariables(currentVariables.size());
	vector<int> greenClauses(this->clauses.size());
	vector<int> greenVariables(this->variables.size());

	vector<int>::iterator it = set_difference(this->parent()->clauses.begin(), this->parent()->clauses.end(), 
							this->clauses.begin(), this->clauses.end(), redClauses.begin());
	redClauses.resize(it - redClauses.begin());

	it = set_difference(this->parent()->variables.begin(), this->parent()->variables.end(),
				this->variables.begin(), this->variables.end(), redVariables.begin());
	redVariables.resize(it - redVariables.begin());

	it = set_difference(this->clauses.begin(), this->clauses.end(),
				this->parent()->clauses.begin(), this->parent()->clauses.end(), greenClauses.begin());
	greenClauses.resize(it - greenClauses.begin());

	it = set_difference(this->variables.begin(), this->variables.end(),
				this->parent()->variables.begin(), this->parent()->variables.end(), greenVariables.begin());
	greenVariables.resize(it - greenVariables.begin());

	changes = redClauses.size() + greenClauses.size() + redVariables.size() + greenVariables.size();

	for(it = redClauses.begin(); changes > 1 && it != redClauses.end(); ++it)
	{
		currentClauses.remove(*it);
		createChild(this, set<int>(currentClauses.begin(), currentClauses.end()), set<int>(currentVariables.begin(), currentVariables.end()));
		--changes;
	}

	for(it = redVariables.begin(); changes > 1 && it != redVariables.end(); ++it)
	{
		currentVariables.remove(*it);
		createChild(this, set<int>(currentClauses.begin(), currentClauses.end()), set<int>(currentVariables.begin(), currentVariables.end()));
		--changes;
	}

	for(it = greenClauses.begin(); changes > 1 && it != greenClauses.end(); ++it)
	{
		currentClauses.push_back(*it);
		createChild(this, set<int>(currentClauses.begin(), currentClauses.end()), set<int>(currentVariables.begin(), currentVariables.end()));
		--changes;
	}
	
	for(it = greenVariables.begin(); changes > 1 && it != greenVariables.end(); ++it)
	{
		currentClauses.push_back(*it);
		createChild(this, set<int>(currentClauses.begin(), currentClauses.end()), set<int>(currentVariables.begin(), currentVariables.end()));
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

const set<int> &ExtendedHypertree::getClauses() const { return this->clauses; }
const set<int> &ExtendedHypertree::getVariables() const { return this->variables; }
