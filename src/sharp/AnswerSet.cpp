#include <iostream>

#include "AnswerSet.h"

using namespace std;

#if defined(VERBOSE) && defined(DEBUG)
static void printTuples(TupleSet *svals, const ExtendedHypertree *node)
{
        cout << "SVALS: " << svals->size() << " for node " << node << endl;
        for(TupleSet::iterator it = svals->begin(); it != svals->end(); ++it)
        {
		AnswerSetTuple &c = *(AnswerSetTuple *)it->first;
                cout << "var: "; printIntSet(c.variables);
                cout << ", cla: "; printIntSet(c.rules);
		cout << ", gua: "; 
		for(set<Atom>::iterator i = c.guards.begin(); i != c.guards.end(); ++i)
		{ 
			cout << "[>"; printIntSet(i->first); 
			cout << "<|>"; printIntSet(i->second); 
			cout << "<]";
		}
		cout << endl;
        }
}
#endif

AnswerSetTuple::AnswerSetTuple() { }

AnswerSetTuple::~AnswerSetTuple() { }

bool AnswerSetTuple::operator<(const Tuple &other) const
{
	AnswerSetTuple &o = (AnswerSetTuple &)other;

	return this->variables < o.variables ||
		(this->variables == o.variables && this->rules < o.rules) ||
		(this->variables == o.variables && this->rules == o.rules && this->guards < o.guards);
}

bool AnswerSetTuple::operator==(const Tuple &other) const
{
	equal_to<set<int> > ieq;
	equal_to<set<Atom> > aeq;
	AnswerSetTuple &o = (AnswerSetTuple &)other;

	return ieq(this->variables, o.variables) &&
		ieq(this->rules, o.rules) &&
		aeq(this->guards, o.guards);
}

int AnswerSetTuple::hash() const
{
	//TODO
	return -1;
}

AnswerSetAlgorithm::AnswerSetAlgorithm(Problem *problem)
	: AbstractAlgorithm(problem)
{
	this->problem = (DatalogProblem *)problem;
}

AnswerSetAlgorithm::~AnswerSetAlgorithm() { }

VariableSet AnswerSetAlgorithm::getVariables(const ExtendedHypertree *node)
{
	VariableSet variables;
	for(VertexSet::iterator it = node->getVertices().begin();
			it != node->getVertices().end();
			++it)
		if(this->problem->isVariable(*it)) variables.insert(*it);
	return variables;
}

RuleSet AnswerSetAlgorithm::getRules(const ExtendedHypertree *node)
{
	RuleSet rules;
	for(VertexSet::iterator it = node->getVertices().begin();
			it != node->getVertices().end();
			++it)
		if(this->problem->isRule(*it)) rules.insert(*it);
	return rules;
}

Solution *AnswerSetAlgorithm::selectSolution(TupleSet *tuples, const ExtendedHypertree *root)
{
	Solution *s = this->instantiator->createEmptySolution();
	equal_to<set<int> > eq;
	RuleSet rules = getRules(root);

	for(TupleSet::iterator it = tuples->begin(); it != tuples->end(); ++it)
	{
		AnswerSetTuple &x = *(AnswerSetTuple *)it->first;

		if(!eq(rules, x.rules)) continue;

		bool filter = false;
		for(set<Atom>::iterator git = x.guards.begin(); 
			!filter && git != x.guards.end(); ++git)
		{
			if(git->second.size() >= rules.size()) { filter = true; continue; }
			
			set<Rule>::iterator rit = rules.begin();
			for(set<Rule>::iterator rgit = git->second.begin(); 
				!filter && rgit != git->second.end(); ++rgit)
			{
				while(rit != rules.end() && *rit < *rgit) ++rit;
				if(rit == rules.end() || *rit != *rgit) filter = true;
			}
		}

		if(!filter) s = this->instantiator->combine(Union, s, it->second);
	}

	return s;
}

TupleSet *AnswerSetAlgorithm::evaluateLeafNode(const ExtendedHypertree *node)
{
	TupleSet *ts = new TupleSet();

	RuleSet nodeRules = getRules(node);
	Partition apart = Helper::partition(getVariables(node));

        for(unsigned int i = 0; i < apart.first.size(); ++i)
	{
		AnswerSetTuple &ast = *new AnswerSetTuple();

		Partition gpart = Helper::partition(apart.first[i]);

		for(unsigned int j = 0; j < gpart.first.size(); ++j)
			if(gpart.first[j].size() != apart.first[i].size())
			{
				gpart.second[j].insert(apart.second[i].begin(), 
						       apart.second[i].end());
				ast.guards.insert(Atom(gpart.first[j],
						       Helper::trueRules(apart.first[i],
								gpart.first[j], 
								gpart.second[j], 
								nodeRules,
								this->problem->getSignMap(),
								this->problem->getHeadMap())));
			}

		//FIXME: use swap instead of copying...
		ast.variables = apart.first[i];
		ast.rules = Helper::trueRules(apart.first[i], 
					      apart.second[i], 
					      nodeRules,
					      this->problem->getSignMap());

		ts->insert(TupleSet::value_type(&ast, 
			this->instantiator->createLeafSolution(apart.first[i])));
	}

#if defined(VERBOSE) && defined(DEBUG)
	printTuples(ts, node);
#endif

        return ts;
}

TupleSet *AnswerSetAlgorithm::evaluateBranchNode(const ExtendedHypertree *node)
{
	TupleSet *left = this->evaluateNode(node->firstChild()), 
		*right = this->evaluateNode(node->secondChild());
	TupleSet *ts = new TupleSet();
	equal_to<set<Variable> > eq;

	for(TupleSet::iterator lit = left->begin(); lit != left->end(); ++lit)
	{
		AnswerSetTuple &l = *(AnswerSetTuple *)lit->first;
		for(TupleSet::iterator rit = right->begin(); rit != right->end(); ++rit)
		{
			AnswerSetTuple &r = *(AnswerSetTuple *)rit->first;
			if(!eq(l.variables, r.variables)) continue;

			AnswerSetTuple &ast = *new AnswerSetTuple();

			for(set<Atom>::iterator glit = l.guards.begin(); 
				glit != l.guards.end(); ++glit)
			{
				if(!eq(r.variables, glit->first)) continue;
				Atom g(r.variables, r.rules);
				g.second.insert(glit->second.begin(), glit->second.end());
				ast.guards.insert(g);
			}

			for(set<Atom>::iterator glit = l.guards.begin(); 
				glit != l.guards.end(); ++glit)
				for(set<Atom>::iterator grit = r.guards.begin(); 
					grit != r.guards.end(); ++grit)
				{
					if(!eq(glit->first, grit->first)) continue;
					Atom g(glit->first, glit->second);
					g.second.insert(grit->second.begin(), 
							grit->second.end());
					ast.guards.insert(g);
				}

			for(set<Atom>::iterator grit = r.guards.begin(); 
				grit != r.guards.end(); ++grit)
			{
				if(!eq(l.variables, grit->first)) continue;
				Atom g(l.variables, l.rules);
				g.second.insert(grit->second.begin(), grit->second.end());
				ast.guards.insert(g);
			}

			//FIXME: use swap instead of copying...
			ast.variables = l.variables;
			//FIXME: use swap instead of copying...
			ast.rules = l.rules;
			ast.rules.insert(r.rules.begin(), r.rules.end());

			Solution *s = this->instantiator->combine(CrossJoin, 
								  lit->second, 
								  rit->second);

			//FIXME implement a postproc function to avoid duplicate code
			pair<TupleSet::iterator, bool> result = 
				ts->insert(TupleSet::value_type(&ast, s));
			if(!result.second)
			{
				Solution *orig = result.first->second;
				ts->erase(result.first);
				ts->insert(TupleSet::value_type(&ast, 
					this->instantiator->combine(Union, orig, s)));
			}
		}
	}

	delete left;
	delete right;

#if defined(VERBOSE) && defined(DEBUG)
	printTuples(ts, node);
#endif

	return ts;
}

TupleSet *AnswerSetAlgorithm::evaluateIntroductionNode(const ExtendedHypertree *node)
{
	if(this->problem->isRule(node->getDifference()))
		return evaluateRuleIntroductionNode(node);
	return evaluateVariableIntroductionNode(node);
}

TupleSet *AnswerSetAlgorithm::evaluateRemovalNode(const ExtendedHypertree *node)
{
	if(this->problem->isRule(node->getDifference()))
		return evaluateRuleRemovalNode(node);
	return evaluateVariableRemovalNode(node);
}

TupleSet *AnswerSetAlgorithm::evaluateVariableIntroductionNode(const ExtendedHypertree *node)
{
	TupleSet *base = this->evaluateNode(node->firstChild());
	TupleSet *ts = new TupleSet();

	RuleSet nodeRules = getRules(node);

	set<Variable> var; var.insert(node->getDifference());
	set<Rule> trueN = Helper::trueRules(set<Variable>(), var, 
				nodeRules, this->problem->getSignMap());
	set<Rule> trueNR = Helper::trueRules(var, set<Variable>(), var, 
				nodeRules, this->problem->getSignMap(), this->problem->getHeadMap());
	set<Rule> trueP = Helper::trueRules(var, set<Variable>(), 
				nodeRules, this->problem->getSignMap());
	set<Rule> truePR = Helper::trueRules(var, var, set<Variable>(),
				nodeRules, this->problem->getSignMap(), this->problem->getHeadMap());

	for(TupleSet::iterator it = base->begin(); it != base->end(); ++it)
	{
		AnswerSetTuple &x = *(AnswerSetTuple *)it->first;
		AnswerSetTuple &astt = *new AnswerSetTuple(),
			&astf = *new AnswerSetTuple();
		
		//FIXME: use swap instead of copying - at least for one set...
		astt.variables = x.variables;
		astt.variables.insert(node->getDifference());
		astf.variables = x.variables;

		//FIXME: use swap instead of copying - at least for one set...
		astt.rules = x.rules;
		astt.rules.insert(trueP.begin(), trueP.end());
		astf.rules = x.rules;
		astf.rules.insert(trueN.begin(), trueN.end());

		for(set<Atom>::iterator git = x.guards.begin(); git != x.guards.end(); ++git)
		{
			//FIXME: use swap instead of copying - at least for one atom...
			Atom 	tempt1(git->first, git->second), 
				tempt2(git->first, git->second),
				tempf(git->first, git->second);
			tempt1.first.insert(node->getDifference());
			tempt1.second.insert(truePR.begin(), truePR.end());
			tempt2.second.insert(trueNR.begin(), trueNR.end());
			tempf.second.insert(trueN.begin(), trueN.end());

			astt.guards.insert(tempt1);
			astt.guards.insert(tempt2);
			astf.guards.insert(tempf);
		}

		Atom temp(x.variables, x.rules);
		temp.second.insert(trueNR.begin(), trueNR.end());
		astt.guards.insert(temp);

		//FIXME implement a postproc function to avoid duplicate code
		pair<TupleSet::iterator, bool> result = 
			ts->insert(TupleSet::value_type(&astf, it->second));
		if(!result.second)
		{
			Solution *orig = result.first->second;
			ts->erase(result.first);
			ts->insert(TupleSet::value_type(&astf, 
				this->instantiator->combine(Union, orig, it->second)));
		}

		Solution *s = this->instantiator->addDifference(it->second, node->getDifference());		

		//FIXME implement a postproc function to avoid duplicate code
		result = ts->insert(TupleSet::value_type(&astt, s));
		if(!result.second)
		{
			Solution *orig = result.first->second;
			ts->erase(result.first);
			ts->insert(TupleSet::value_type(&astt, 
				this->instantiator->combine(Union, orig, s)));
		}
	}

	delete base;

#if defined(VERBOSE) && defined(DEBUG)
	printTuples(ts, node);
#endif

	return ts;
}

TupleSet *AnswerSetAlgorithm::evaluateVariableRemovalNode(const ExtendedHypertree *node)
{
	TupleSet *base = this->evaluateNode(node->firstChild());
	TupleSet *ts = new TupleSet();

	for(TupleSet::iterator it = base->begin(); it != base->end(); ++it)
	{
		AnswerSetTuple &x = *(AnswerSetTuple *)it->first;
		AnswerSetTuple &ast = *new AnswerSetTuple();

		//FIXME: use swap/erase instead of manual filtering...
		for(set<Variable>::iterator vit = x.variables.begin(); 
			vit != x.variables.end(); ++vit)
			if(*vit != node->getDifference()) ast.variables.insert(*vit);
		
		//FIXME: use swap instead of copying...
		ast.rules = x.rules;

		//FIXME: use swap/erase instead of manual filtering...
		for(set<Atom>::iterator git = x.guards.begin(); git != x.guards.end(); ++git)
		{
			//FIXME: use swap instead of copying...
			Atom temp(set<Variable>(), git->second);
			for(set<Variable>::iterator vgit = git->first.begin(); 
				vgit != git->first.end(); ++vgit)
				if(*vgit != node->getDifference()) temp.first.insert(*vgit);
			ast.guards.insert(temp);
		}

		//FIXME implement a postproc function to avoid duplicate code
		pair<TupleSet::iterator, bool> result = 
			ts->insert(TupleSet::value_type(&ast, it->second));
		if(!result.second)
		{
			Solution *orig = result.first->second;
			ts->erase(result.first);
			ts->insert(TupleSet::value_type(&ast, 
				this->instantiator->combine(Union, orig, it->second)));
		}
	}

	delete base;

#if defined(VERBOSE) && defined(DEBUG)
	printTuples(ts, node);
#endif

	return ts;
}

TupleSet *AnswerSetAlgorithm::evaluateRuleIntroductionNode(const ExtendedHypertree *node)
{
	TupleSet *base = this->evaluateNode(node->firstChild());
	TupleSet *ts = new TupleSet();

	VariableSet nodeVariables = getVariables(node);

	for(TupleSet::iterator it = base->begin(); it != base->end(); ++it)
	{
		AnswerSetTuple &x = *(AnswerSetTuple *)it->first;
		AnswerSetTuple &ast = *new AnswerSetTuple();
		//FIXME: use swap instead of copying...
		ast.variables = x.variables;

		//FIXME: use swap instead of copying...
		ast.rules = x.rules;
		if(Helper::trueRule(ast.variables, 
				    nodeVariables, 
				    node->getDifference(),
				    this->problem->getSignMap())) 
			ast.rules.insert(node->getDifference());
		for(set<Atom>::iterator git = x.guards.begin(); git != x.guards.end(); ++git)
		{
			//FIXME: use swap instead of copying...
			Atom temp(git->first, git->second);
			if(Helper::trueRule(x.variables,
					    temp.first, 
					    nodeVariables,
					    node->getDifference(),
					    this->problem->getSignMap(),
					    this->problem->getHeadMap())) 
				temp.second.insert(node->getDifference());
			ast.guards.insert(temp);
		}

		//FIXME implement a postproc function to avoid duplicate code
		pair<TupleSet::iterator, bool> result = 
			ts->insert(TupleSet::value_type(&ast, it->second));
		if(!result.second)
		{
			Solution *orig = result.first->second;
			ts->erase(result.first);
			ts->insert(TupleSet::value_type(&ast, 
				this->instantiator->combine(Union, orig, it->second)));
		}
	}

	delete base;

#if defined(VERBOSE) && defined(DEBUG)
	printTuples(ts, node);
#endif

	return ts;
}

TupleSet *AnswerSetAlgorithm::evaluateRuleRemovalNode(const ExtendedHypertree *node)
{
	TupleSet *base = this->evaluateNode(node->firstChild());
	TupleSet *ts = new TupleSet();

	for(TupleSet::iterator it = base->begin(); it != base->end(); ++it)
	{
		AnswerSetTuple &x = *(AnswerSetTuple *)it->first;
		AnswerSetTuple &ast = *new AnswerSetTuple();

		if(x.rules.find(node->getDifference()) == x.rules.end()) continue;
		
		//FIXME: use swap instead of copying...
		ast.variables = x.variables;
		
		ast.rules.swap(x.rules);
		ast.rules.erase(node->getDifference());

		//FIXME: just erase the one element... what the hell?
		//for(set<Rule>::iterator rit = x.rules.begin(); rit != x.rules.end(); ++rit)
		//	if(*rit != node->getDifference()) ast.rules.insert(*rit);

		//FIXME: use swap/erase instead of manual filtering...
		for(set<Atom>::iterator git = x.guards.begin(); git != x.guards.end(); ++git)
		{
			if(git->second.find(node->getDifference()) == git->second.end()) 
				continue;

			//FIXME: use swap instead of copying...
			Atom temp(git->first, set<Rule>());
			for(set<Rule>::iterator rgit = git->second.begin(); 
				rgit != git->second.end(); ++rgit)
				if(*rgit != node->getDifference()) temp.second.insert(*rgit);
			ast.guards.insert(temp);
		}
		
		//FIXME implement a postproc function to avoid duplicate code
		pair<TupleSet::iterator, bool> result = 
			ts->insert(TupleSet::value_type(&ast, it->second));
		if(!result.second)
		{
			Solution *orig = result.first->second;
			ts->erase(result.first);
			ts->insert(TupleSet::value_type(&ast, 
				this->instantiator->combine(Union, orig, it->second)));
		}
	}

	delete base;

#if defined(VERBOSE) && defined(DEBUG)
	printTuples(ts, node);
#endif

	return ts;
}

