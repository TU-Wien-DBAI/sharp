#include <iostream>

#include "Circumscription.h"

using namespace std;

#if defined(VERBOSE) && defined(DEBUG)
static void printSolution(Solution *solution)
{
	EnumerationSolutionContent *s = 
		(EnumerationSolutionContent *)solution->getContent();

	cout << "[";	
	for(set<set<Variable> >::iterator it = s->enumerations.begin(); 
		it != s->enumerations.end(); ++it)
        {
                cout << "["; printIntSet(*it);
                cout << "]";
        }
	cout << "]";
}

static void printTuples(TupleSet *svals, const ExtendedHypertree *node)
{
        cout << "SVALS: " << svals->size() << " for node " << node << endl;
        for(TupleSet::iterator it = svals->begin(); it != svals->end(); ++it)
        {
		CircumscriptionTuple &c = *(CircumscriptionTuple *)it->first;
                cout << "var: "; printIntSet(c.variables);
                cout << ", cla: "; printIntSet(c.rules);
		cout << ", gua: "; 
		for(set<Atom>::iterator i = c.guards.begin(); i != c.guards.end(); ++i)
		{ 
			cout << "[>"; printIntSet(i->first); 
			cout << "<|>"; printIntSet(i->second); 
			cout << "<]";
		}
		cout << ", sol: ";
		printSolution(it->second);
		cout << endl;
        }
}
#endif

CircumscriptionTuple::CircumscriptionTuple() { }

CircumscriptionTuple::~CircumscriptionTuple() { }

bool CircumscriptionTuple::operator<(const Tuple &other) const
{
	CircumscriptionTuple &o = (CircumscriptionTuple &)other;

	return this->variables < o.variables ||
		(this->variables == o.variables && this->rules < o.rules) ||
		(this->variables == o.variables && this->rules == o.rules && this->guards < o.guards);
}

bool CircumscriptionTuple::operator==(const Tuple &other) const
{
	equal_to<set<int> > ieq;
	equal_to<set<Atom> > aeq;
	CircumscriptionTuple &o = (CircumscriptionTuple &)other;

	return ieq(this->variables, o.variables) &&
		ieq(this->rules, o.rules) &&
		aeq(this->guards, o.guards);
}

int CircumscriptionTuple::hash() const
{
	//TODO
	return -1;
}

CircumscriptionAlgorithm::CircumscriptionAlgorithm
	(const Instantiator *instantiator, const ExtendedHypertree *root, 
		const SignMap &signMap, const HeadMap &headMap, const NameMap &nameMap)
	: AbstractAlgorithm(instantiator, root, signMap, headMap, nameMap)
{
}

CircumscriptionAlgorithm::~CircumscriptionAlgorithm() { }

Solution *CircumscriptionAlgorithm::selectSolution(TupleSet *tuples)
{
	Solution *s = this->instantiator->createEmptySolution();
	equal_to<set<int> > eq;
	set<Rule> rules = this->root->getRules();

	for(TupleSet::iterator it = tuples->begin(); it != tuples->end(); ++it)
	{
		CircumscriptionTuple &x = *(CircumscriptionTuple *)it->first;

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

TupleSet *CircumscriptionAlgorithm::evaluateLeafNode(const ExtendedHypertree *node)
{
	TupleSet *ts = new TupleSet();

	Partition apart = Helper::partition(node->getVariables());

        for(unsigned int i = 0; i < apart.first.size(); ++i)
	{
		CircumscriptionTuple &cet = *new CircumscriptionTuple();

		Partition gpart = Helper::partition(apart.first[i]);

		for(unsigned int j = 0; j < gpart.first.size(); ++j)
			if(gpart.first[j].size() != apart.first[i].size())
			{
				gpart.second[j].insert(apart.second[i].begin(), 
						       apart.second[i].end());
				cet.guards.insert(Atom(gpart.first[j],
						       Helper::trueRules(gpart.first[j], 
								   gpart.second[j], 
								   node->getRules(),
								   this->signMap)));
			}

		//FIXME: use swap instead of copying...
		cet.variables = apart.first[i];
		cet.rules = Helper::trueRules(apart.first[i], 
					      apart.second[i], 
					      node->getRules(),
					      this->signMap);

		ts->insert(TupleSet::value_type(&cet, 
			this->instantiator->createLeafSolution(apart.first[i])));
	}

#if defined(VERBOSE) && defined(DEBUG)
	printTuples(ts, node);
#endif

        return ts;
}

TupleSet *CircumscriptionAlgorithm::evaluateBranchNode(const ExtendedHypertree *node)
{
	TupleSet *left = this->evaluateNode(node->firstChild()), 
		*right = this->evaluateNode(node->secondChild());
	TupleSet *ts = new TupleSet();
	equal_to<set<Variable> > eq;

	for(TupleSet::iterator lit = left->begin(); lit != left->end(); ++lit)
	{
		CircumscriptionTuple &l = *(CircumscriptionTuple *)lit->first;
		for(TupleSet::iterator rit = right->begin(); rit != right->end(); ++rit)
		{
			CircumscriptionTuple &r = *(CircumscriptionTuple *)rit->first;
			if(!eq(l.variables, r.variables)) continue;

			CircumscriptionTuple &cet = *new CircumscriptionTuple();

			for(set<Atom>::iterator glit = l.guards.begin(); 
				glit != l.guards.end(); ++glit)
			{
				if(!eq(r.variables, glit->first)) continue;
				Atom g(r.variables, r.rules);
				g.second.insert(glit->second.begin(), glit->second.end());
				cet.guards.insert(g);
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
					cet.guards.insert(g);
				}

			for(set<Atom>::iterator grit = r.guards.begin(); 
				grit != r.guards.end(); ++grit)
			{
				if(!eq(l.variables, grit->first)) continue;
				Atom g(l.variables, l.rules);
				g.second.insert(grit->second.begin(), grit->second.end());
				cet.guards.insert(g);
			}

			//FIXME: use swap instead of copying...
			cet.variables = l.variables;
			//FIXME: use swap instead of copying...
			cet.rules = l.rules;
			cet.rules.insert(r.rules.begin(), r.rules.end());

			Solution *s = this->instantiator->combine(CrossJoin, 
								  lit->second, 
								  rit->second);

			//FIXME implement a postproc function to avoid duplicate code
			pair<TupleSet::iterator, bool> result = 
				ts->insert(TupleSet::value_type(&cet, s));
			if(!result.second)
			{
				Solution *orig = result.first->second;
				ts->erase(result.first);
				ts->insert(TupleSet::value_type(&cet, 
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

TupleSet *CircumscriptionAlgorithm::evaluateVariableIntroductionNode(const ExtendedHypertree *node)
{
	TupleSet *base = this->evaluateNode(node->firstChild());
	TupleSet *ts = new TupleSet();

	set<Variable> var; var.insert(node->getDifference());
	set<Rule> trueN = Helper::trueRules(set<Variable>(), var, node->getRules(), this->signMap);
	set<Rule> trueP = Helper::trueRules(var, set<Variable>(), node->getRules(), this->signMap);

	for(TupleSet::iterator it = base->begin(); it != base->end(); ++it)
	{
		CircumscriptionTuple &x = *(CircumscriptionTuple *)it->first;
		CircumscriptionTuple &cett = *new CircumscriptionTuple(),
			&cetf = *new CircumscriptionTuple();
		
		//FIXME: use swap instead of copying - at least for one set...
		cett.variables = x.variables;
		cett.variables.insert(node->getDifference());
		cetf.variables = x.variables;

		//FIXME: use swap instead of copying - at least for one set...
		cett.rules = x.rules;
		cett.rules.insert(trueP.begin(), trueP.end());
		cetf.rules = x.rules;
		cetf.rules.insert(trueN.begin(), trueN.end());

		for(set<Atom>::iterator git = x.guards.begin(); git != x.guards.end(); ++git)
		{
			//FIXME: use swap instead of copying - at least for one atom...
			Atom tempt(git->first, git->second), tempf(git->first, git->second);
			tempt.first.insert(node->getDifference());
			tempt.second.insert(trueP.begin(), trueP.end());
			tempf.second.insert(trueN.begin(), trueN.end());

			cett.guards.insert(tempt);
			cett.guards.insert(tempf);
			cetf.guards.insert(tempf);
		}

		cett.guards.insert(Atom(cetf.variables, cetf.rules));

		//FIXME implement a postproc function to avoid duplicate code
		pair<TupleSet::iterator, bool> result = 
			ts->insert(TupleSet::value_type(&cetf, it->second));
		if(!result.second)
		{
			Solution *orig = result.first->second;
			ts->erase(result.first);
			ts->insert(TupleSet::value_type(&cetf, 
				this->instantiator->combine(Union, orig, it->second)));
		}

		Solution *s = this->instantiator->addDifference(it->second, node->getDifference());		
		//FIXME implement a postproc function to avoid duplicate code
		result = ts->insert(TupleSet::value_type(&cett, s));
		if(!result.second)
		{
			Solution *orig = result.first->second;
			ts->erase(result.first);
			ts->insert(TupleSet::value_type(&cett, 
				this->instantiator->combine(Union, orig, it->second)));
		}
	}

	delete base;

#if defined(VERBOSE) && defined(DEBUG)
	printTuples(ts, node);
#endif

	return ts;
}

TupleSet *CircumscriptionAlgorithm::evaluateVariableRemovalNode(const ExtendedHypertree *node)
{
	TupleSet *base = this->evaluateNode(node->firstChild());
	TupleSet *ts = new TupleSet();

	for(TupleSet::iterator it = base->begin(); it != base->end(); ++it)
	{
		CircumscriptionTuple &x = *(CircumscriptionTuple *)it->first;
		CircumscriptionTuple &cet = *new CircumscriptionTuple();

		//FIXME: use swap/erase instead of manual filtering...
		for(set<Variable>::iterator vit = x.variables.begin(); 
			vit != x.variables.end(); ++vit)
			if(*vit != node->getDifference()) cet.variables.insert(*vit);
		
		//FIXME: use swap instead of copying...
		cet.rules = x.rules;

		//FIXME: use swap/erase instead of manual filtering...
		for(set<Atom>::iterator git = x.guards.begin(); git != x.guards.end(); ++git)
		{
			//FIXME: use swap instead of copying...
			Atom temp(set<Variable>(), git->second);
			for(set<Variable>::iterator vgit = git->first.begin(); 
				vgit != git->first.end(); ++vgit)
				if(*vgit != node->getDifference()) temp.first.insert(*vgit);
			cet.guards.insert(temp);
		}

		//FIXME implement a postproc function to avoid duplicate code
		pair<TupleSet::iterator, bool> result = 
			ts->insert(TupleSet::value_type(&cet, it->second));
		if(!result.second)
		{
			Solution *orig = result.first->second;
			ts->erase(result.first);
			ts->insert(TupleSet::value_type(&cet, 
				this->instantiator->combine(Union, orig, it->second)));
		}
	}

	delete base;

#if defined(VERBOSE) && defined(DEBUG)
	printTuples(ts, node);
#endif

	return ts;
}

TupleSet *CircumscriptionAlgorithm::evaluateRuleIntroductionNode(const ExtendedHypertree *node)
{
	TupleSet *base = this->evaluateNode(node->firstChild());
	TupleSet *ts = new TupleSet();

	for(TupleSet::iterator it = base->begin(); it != base->end(); ++it)
	{
		CircumscriptionTuple &x = *(CircumscriptionTuple *)it->first;
		CircumscriptionTuple &cet = *new CircumscriptionTuple();
		
		//FIXME: use swap instead of copying...
		cet.variables = x.variables;

		//FIXME: use swap instead of copying...
		cet.rules = x.rules;
		if(Helper::trueRule(cet.variables, 
				    node->getVariables(), 
				    node->getDifference(),
				    this->signMap)) 
			cet.rules.insert(node->getDifference());

		for(set<Atom>::iterator git = x.guards.begin(); git != x.guards.end(); ++git)
		{
			//FIXME: use swap instead of copying...
			Atom temp(git->first, git->second);
			if(Helper::trueRule(temp.first, 
					    node->getVariables(), 
					    node->getDifference(),
					    this->signMap)) 
				temp.second.insert(node->getDifference());
			cet.guards.insert(temp);
		}

		//FIXME implement a postproc function to avoid duplicate code
		pair<TupleSet::iterator, bool> result = 
			ts->insert(TupleSet::value_type(&cet, it->second));
		if(!result.second)
		{
			Solution *orig = result.first->second;
			ts->erase(result.first);
			ts->insert(TupleSet::value_type(&cet, 
				this->instantiator->combine(Union, orig, it->second)));
		}
	}

	delete base;

#if defined(VERBOSE) && defined(DEBUG)
	printTuples(ts, node);
#endif

	return ts;
}

TupleSet *CircumscriptionAlgorithm::evaluateRuleRemovalNode(const ExtendedHypertree *node)
{
	TupleSet *base = this->evaluateNode(node->firstChild());
	TupleSet *ts = new TupleSet();

	for(TupleSet::iterator it = base->begin(); it != base->end(); ++it)
	{
		CircumscriptionTuple &x = *(CircumscriptionTuple *)it->first;
		CircumscriptionTuple &cet = *new CircumscriptionTuple();

		if(x.rules.find(node->getDifference()) == x.rules.end()) continue;
		
		//FIXME: use swap instead of copying...
		cet.variables = x.variables;
		
		cet.rules.swap(x.rules);
		cet.rules.erase(node->getDifference());

		//FIXME: just erase the one element... what the hell?
		//for(set<Rule>::iterator rit = x.rules.begin(); rit != x.rules.end(); ++rit)
		//	if(*rit != node->getDifference()) cet.rules.insert(*rit);

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
			cet.guards.insert(temp);
		}
		
		//FIXME implement a postproc function to avoid duplicate code
		pair<TupleSet::iterator, bool> result = 
			ts->insert(TupleSet::value_type(&cet, it->second));
		if(!result.second)
		{
			Solution *orig = result.first->second;
			ts->erase(result.first);
			ts->insert(TupleSet::value_type(&cet, 
				this->instantiator->combine(Union, orig, it->second)));
		}
	}

	delete base;

#if defined(VERBOSE) && defined(DEBUG)
	printTuples(ts, node);
#endif

	return ts;
}

