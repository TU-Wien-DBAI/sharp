#include <iostream>
#include <algorithm>

#include "HeadCycleFreeAnswerSet.h"

using namespace std;

#if defined(VERBOSE) && defined(DEBUG)
static void printTuples(TupleSet *svals, const ExtendedHypertree *node)
{
        cout << "SVALS: " << svals->size() << " for node " << node << endl;
        for(TupleSet::iterator it = svals->begin(); it != svals->end(); ++it)
        {
		HeadCycleFreeAnswerSetTuple &c = *(HeadCycleFreeAnswerSetTuple *)it->first;
                cout << "var: "; printIntSet(c.variables);
                cout << ", cla: "; printIntSet(c.rules);
		cout << ", ord: "; printIntList(c.order);
		cout << ", typ: "; printBoolList(c.ordertypes); 
		cout << ", gua: ";
		for(map<Variable, set<Rule> >::iterator i = c.guards.begin(); i != c.guards.end(); ++i)
		{ 
			cout << "[" << i->first << "|>"; printIntSet(i->second); cout << "<]";
		}
		cout << ", glo: ";
		for(set<set<Rule> >::iterator i = c.guardsdown.begin(); i != c.guardsdown.end(); ++i)
		{
			cout << "["; printIntSet(*i); cout << "]";
		}
		cout << endl;
        }
}
#endif

HeadCycleFreeAnswerSetTuple::HeadCycleFreeAnswerSetTuple() { }

HeadCycleFreeAnswerSetTuple::~HeadCycleFreeAnswerSetTuple() { }

bool HeadCycleFreeAnswerSetTuple::operator<(const Tuple &other) const
{
	HeadCycleFreeAnswerSetTuple &o = (HeadCycleFreeAnswerSetTuple &)other;
	//TODO

	return     (this->variables < o.variables
		|| (this->variables == o.variables
		&& (this->rules < o.rules
		|| (this->rules == o.rules
		&& (this->order < o.order
		|| (this->order == o.order
		&& (this->ordertypes < o.ordertypes
		|| (this->ordertypes == o.ordertypes
		&& (this->guards < o.guards
		|| (this->guards == o.guards
		&& (this->guardsdown < o.guardsdown)))))))))));
}

bool HeadCycleFreeAnswerSetTuple::operator==(const Tuple &other) const
{
	HeadCycleFreeAnswerSetTuple &o = (HeadCycleFreeAnswerSetTuple &)other;
	//TODO

	return 	   this->variables == o.variables
		&& this->rules == o.rules
		&& this->order == o.order
		&& this->ordertypes == o.ordertypes
		&& this->guards == o.guards
		&& this->guardsdown == o.guardsdown;
}

int HeadCycleFreeAnswerSetTuple::hash() const
{
	//TODO
	return -1;
}

HeadCycleFreeAnswerSetAlgorithm::HeadCycleFreeAnswerSetAlgorithm
	(const Instantiator *instantiator, const ExtendedHypertree *root, 
		const SignMap &signMap, const HeadMap &headMap, const NameMap &nameMap)
	: AbstractAlgorithm(instantiator, root, signMap, headMap, nameMap)
{
}

HeadCycleFreeAnswerSetAlgorithm::~HeadCycleFreeAnswerSetAlgorithm() { }

Solution *HeadCycleFreeAnswerSetAlgorithm::selectSolution(TupleSet *tuples)
{
	Solution *s = this->instantiator->createEmptySolution();

	//TODO

	equal_to<set<int> > eq;
	set<Rule> rules = this->root->getRules();

	for(TupleSet::iterator it = tuples->begin(); it != tuples->end(); ++it)
	{
		HeadCycleFreeAnswerSetTuple &x = *(HeadCycleFreeAnswerSetTuple *)it->first;

		if(!eq(rules, x.rules)) continue;

		bool filter = false;
		for(map<Variable, set<Rule> >::iterator git = x.guards.begin(); 
			!filter && git != x.guards.end(); ++git)
		{
			if(git->second.size() == 0) filter = true;
		}

		if(!filter) s = this->instantiator->combine(Union, s, it->second);
	}

	return s;

}

TupleSet *HeadCycleFreeAnswerSetAlgorithm::evaluateLeafNode(const ExtendedHypertree *node)
{
	TupleSet *ts = new TupleSet();

	Partition apart = Helper::partition(node->getVariables());

        for(unsigned int i = 0; i < apart.first.size(); ++i)
	{
		list<Variable> order(apart.first[i].begin(), apart.first[i].end());

		do
		{
			HeadCycleFreeAnswerSetTuple &ast = *new HeadCycleFreeAnswerSetTuple();

			Partition gpart = Helper::partition(apart.first[i]);

			ast.variables = apart.first[i];
			ast.rules = Helper::trueRules(apart.first[i], 
					      apart.second[i], 
					      node->getRules(),
					      this->signMap);

			ast.order = order;
			ast.ordertypes = OrderTypes(order.size(), false);

			for(set<Variable>::iterator it = ast.variables.begin();
				it != ast.variables.end();
				++it)
			{
				ast.guards.insert(pair<Variable, set<Rule> >(
					*it,
					Helper::trueRules(apart.first[i],
						node->getVariables(),
						node->getRules(),
						*it,
						ast.order,
						ast.ordertypes,
						this->signMap,
						this->headMap
					)));
			}

			ts->insert(TupleSet::value_type(&ast, 
				this->instantiator->createLeafSolution(apart.first[i])));
			
		}
		while(next_permutation(order.begin(), order.end()));

	}

#if defined(VERBOSE) && defined(DEBUG)
	printTuples(ts, node);
#endif

        return ts;
}

TupleSet *HeadCycleFreeAnswerSetAlgorithm::evaluateBranchNode(const ExtendedHypertree *node)
{
	TupleSet *left = this->evaluateNode(node->firstChild()), 
		*right = this->evaluateNode(node->secondChild());
	TupleSet *ts = new TupleSet();
	equal_to<set<Variable> > eq;

	for(TupleSet::iterator lit = left->begin(); lit != left->end(); ++lit)
	{
		HeadCycleFreeAnswerSetTuple &l = *(HeadCycleFreeAnswerSetTuple *)lit->first;
		for(TupleSet::iterator rit = right->begin(); rit != right->end(); ++rit)
		{
			HeadCycleFreeAnswerSetTuple &r = *(HeadCycleFreeAnswerSetTuple *)rit->first;
			if(!eq(l.variables, r.variables)) continue;

			//FIXME remove this, consistency checking is implemented in combineOrder(...)
			//if(!Helper::isConsistent(l.order, l.ordertypes, r.order, r.ordertypes)) continue;

			OrderCombinations combinations = Helper::combineOrder(l.order, l.ordertypes,
										r.order, r.ordertypes);

			for(OrderCombinations::iterator oit = combinations.begin();
				oit != combinations.end();
				++oit)
			{

				HeadCycleFreeAnswerSetTuple &ast = *new HeadCycleFreeAnswerSetTuple();
				bool insert = true;
	
				ast.variables = l.variables;
				
				ast.rules = l.rules;
				ast.rules.insert(r.rules.begin(), r.rules.end());

				ast.order = oit->first;
				ast.ordertypes = oit->second;

				insert_iterator<map<Variable, set<Rule> > > ins(ast.guards, 
										ast.guards.begin());

				map<Variable, set<Rule> >::const_iterator clgit = l.guards.begin(),
									crgit = r.guards.begin();
				
				while(clgit != l.guards.end() && crgit != r.guards.end())
				{
					if(clgit->first < crgit->first) ++clgit;
					else if(crgit->first < clgit->first) ++crgit;
					else
					{
						set<Rule> intersect;
						set_intersection(clgit->second.begin(),
								clgit->second.end(),
								crgit->second.begin(),
								crgit->second.end(),
								inserter(intersect, intersect.begin()));

						*ins++ = map<Variable, set<Rule> >::value_type(
								clgit++->first,
								intersect);
					}
				}

				//FIXME the next two for-loops do essentially the same thing -> method
				for(set<set<Rule> >::const_iterator lgit = l.guardsdown.begin(); 
					lgit != l.guardsdown.end(); 
					++lgit)
				{
					set<Rule> diff;
					insert_iterator<set<Rule> > ii = inserter(diff, diff.begin());
					set_difference(lgit->begin(), lgit->end(), 
						r.rules.begin(), r.rules.end(), ii);

					if(diff.size() == 0) { insert = false; break; }
					ast.guardsdown.insert(diff);					
				}

				for(set<set<Rule> >::const_iterator rgit = r.guardsdown.begin(); 
					insert && rgit != r.guardsdown.end(); 
					++rgit)
				{
					set<Rule> diff;
					insert_iterator<set<Rule> > ii = inserter(diff, diff.begin());
					set_difference(rgit->begin(), rgit->end(),
						l.rules.begin(), l.rules.end(), ii);

					if(diff.size() == 0) { insert = false; break; }
					ast.guardsdown.insert(diff);
				}
				
	
				if(insert)
				{
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
							this->instantiator->combine(Union, 
								orig, s)));
					}
				}
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

TupleSet *HeadCycleFreeAnswerSetAlgorithm::evaluateVariableIntroductionNode(const ExtendedHypertree *node)
{
	TupleSet *base = this->evaluateNode(node->firstChild());
	TupleSet *ts = new TupleSet();

	set<Variable> var; var.insert(node->getDifference());
	set<Rule> trueN = Helper::trueRules(set<Variable>(), var, 
				node->getRules(), this->signMap);
	set<Rule> trueP = Helper::trueRules(var, set<Variable>(), 
				node->getRules(), this->signMap);

	for(TupleSet::iterator it = base->begin(); it != base->end(); ++it)
	{
		HeadCycleFreeAnswerSetTuple &x = *(HeadCycleFreeAnswerSetTuple *)it->first;
		HeadCycleFreeAnswerSetTuple &astf = *new HeadCycleFreeAnswerSetTuple();
		
		astf.variables = x.variables;

		astf.rules = x.rules;
		astf.rules.insert(trueN.begin(), trueN.end());

		astf.order = x.order;
		astf.ordertypes = x.ordertypes;

		bool insert = true;

		//FIXME pull these two for-loops in a separate method, duplicate code
		for(map<Variable, set<Rule> >::iterator git = x.guards.begin();
			git != x.guards.end();
			++git)
		{
			astf.guards[git->first] = Helper::trueRules(	astf.variables,
									node->getVariables(),
									git->second,
									git->first,
									astf.order,
									astf.ordertypes,
									this->getSignMap(),
									this->getHeadMap());
		}

		for(set<set<Rule> >::iterator git = x.guardsdown.begin();
			git != x.guardsdown.end();
			++git)
		{
			set<Rule> next = Helper::trueRules(	astf.variables,
								node->getVariables(),
								*git,
								astf.order,
								astf.ordertypes,	
								this->getSignMap(),
								this->getHeadMap());

			if(next.size() == 0) { insert = false; break; }
			
			astf.guardsdown.insert(next);
		}

		if(insert)
		{
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
		}

		OrderCombinations combinations = 
			Helper::combineOrder(x.order, x.ordertypes, node->getDifference(), false);

		for(OrderCombinations::iterator oit = combinations.begin(); 
			oit != combinations.end();
			++oit)
		{
			HeadCycleFreeAnswerSetTuple &astt = *new HeadCycleFreeAnswerSetTuple();

			insert = true;

			astt.variables = x.variables;
			astt.variables.insert(node->getDifference());

			astt.rules = x.rules;
			astt.rules.insert(trueP.begin(), trueP.end());

			astt.order = oit->first;
			astt.ordertypes = oit->second;

			//FIXME pull these two for-loops in a separate method, duplicate code
			for(map<Variable, set<Rule> >::iterator git = x.guards.begin();
				git != x.guards.end();
				++git)
			{
				astt.guards[git->first] = Helper::trueRules(	astt.variables,
										node->getVariables(),
										git->second,
										git->first,
										astt.order,
										astt.ordertypes,
										this->getSignMap(),
										this->getHeadMap());
			}

			astt.guards[node->getDifference()] = Helper::trueRules(astt.variables,
										node->getVariables(),
										trueP,
										node->getDifference(),
										astt.order,
										astt.ordertypes,
										this->getSignMap(),
										this->getHeadMap());
	
			for(set<set<Rule> >::iterator git = x.guardsdown.begin();
				git != x.guardsdown.end();
				++git)
			{
				set<Rule> next = Helper::trueRules(	astt.variables,
									node->getVariables(),
									*git,
									astt.order,
									astt.ordertypes,	
									this->getSignMap(),
									this->getHeadMap());
	
				if(next.size() == 0) { insert = false; break; }
				
				astt.guardsdown.insert(next);
			}

			if(insert)
			{
				Solution *s = this->instantiator->addDifference(it->second,
									node->getDifference());

				//FIXME implement a postproc function to avoid duplicate code
				pair<TupleSet::iterator, bool> result = 
					ts->insert(TupleSet::value_type(&astt, s));
				if(!result.second)
				{
					Solution *orig = result.first->second;
					ts->erase(result.first);
					ts->insert(TupleSet::value_type(&astt, 
						this->instantiator->combine(Union, 
							orig, it->second)));
				}
			}			
		}
	}

	delete base;

#if defined(VERBOSE) && defined(DEBUG)
	printTuples(ts, node);
#endif

	return ts;
}

TupleSet *HeadCycleFreeAnswerSetAlgorithm::evaluateVariableRemovalNode(const ExtendedHypertree *node)
{
	TupleSet *base = this->evaluateNode(node->firstChild());
	TupleSet *ts = new TupleSet();

	for(TupleSet::iterator it = base->begin(); it != base->end(); ++it)
	{
		HeadCycleFreeAnswerSetTuple &x = *(HeadCycleFreeAnswerSetTuple *)it->first;

		map<Variable, set<Rule> >::iterator git = x.guards.find(node->getDifference());
		set<Rule> l1, l2;

		if(git != x.guards.end())
		{
			if(git->second.size() == 0) continue;
			else l2 = git->second;
		}

		for(set<Rule>::iterator rit = x.rules.begin(); rit != x.rules.end(); ++rit)
		{
			map<Variable, bool>::iterator rulesigns;
			if((rulesigns = this->signMap[*rit].find(node->getDifference())) 
				!= this->signMap[*rit].end())
			{
				if(rulesigns->second) l1.insert(*rit);
			}
		}

		OrderCombinations combinations =
			Helper::combineOrder(x.order, x.ordertypes, l1, l2, node->getDifference(), false);

		for(OrderCombinations::iterator cit = combinations.begin();
			cit != combinations.end();
			++cit)
		{
			HeadCycleFreeAnswerSetTuple &ast = *new HeadCycleFreeAnswerSetTuple();
			ast.guardsdown = x.guardsdown;
	
			if(l2.size() != 0) ast.guardsdown.insert(l2);
	
			for(set<Variable>::iterator vit = x.variables.begin(); 
				vit != x.variables.end(); ++vit)
				if(*vit != node->getDifference()) ast.variables.insert(*vit);
			
			ast.rules = x.rules;

			ast.order = cit->first;
			ast.ordertypes = cit->second;
	
			for(git = x.guards.begin(); git != x.guards.end(); ++git)
			{
				if(git->first == node->getDifference()) continue;
				ast.guards.insert(*git);
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
	}

	delete base;

#if defined(VERBOSE) && defined(DEBUG)
	printTuples(ts, node);
#endif

	return ts;
}

TupleSet *HeadCycleFreeAnswerSetAlgorithm::evaluateRuleIntroductionNode(const ExtendedHypertree *node)
{
	TupleSet *base = this->evaluateNode(node->firstChild());
	TupleSet *ts = new TupleSet();

	for(TupleSet::iterator it = base->begin(); it != base->end(); ++it)
	{
		HeadCycleFreeAnswerSetTuple &x = *(HeadCycleFreeAnswerSetTuple *)it->first;
		HeadCycleFreeAnswerSetTuple &ast = *new HeadCycleFreeAnswerSetTuple();
		
		//FIXME: use swap instead of copying...
		ast.variables = x.variables;

		//FIXME: use swap instead of copying...
		ast.rules = x.rules;
		if(Helper::trueRule(ast.variables, 
				    node->getVariables(), 
				    node->getDifference(),
				    this->signMap)) 
			ast.rules.insert(node->getDifference());

		ast.order = x.order;
		ast.ordertypes = x.ordertypes;

		for(map<Variable, set<Rule> >::iterator git = x.guards.begin(); 
			git != x.guards.end(); 
			++git)
		{
			pair<Variable, set<Rule> > temp(git->first, git->second);
			if(Helper::trueRule(ast.variables,
					    node->getVariables(), 
					    node->getDifference(),
					    temp.first, 
					    ast.order,
					    ast.ordertypes,
					    this->signMap,
					    this->headMap)) 
				temp.second.insert(node->getDifference());
			ast.guards.insert(temp);
		}

		ast.guardsdown = x.guardsdown;

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

TupleSet *HeadCycleFreeAnswerSetAlgorithm::evaluateRuleRemovalNode(const ExtendedHypertree *node)
{
	TupleSet *base = this->evaluateNode(node->firstChild());
	TupleSet *ts = new TupleSet();

	for(TupleSet::iterator it = base->begin(); it != base->end(); ++it)
	{
		HeadCycleFreeAnswerSetTuple &x = *(HeadCycleFreeAnswerSetTuple *)it->first;
		HeadCycleFreeAnswerSetTuple &ast = *new HeadCycleFreeAnswerSetTuple();

		if(x.rules.find(node->getDifference()) == x.rules.end()) continue;
		
		//FIXME: use swap instead of copying...
		ast.variables = x.variables;
		
		ast.rules.swap(x.rules);
		ast.rules.erase(node->getDifference());

		//FIXME: is the above faster than this?
		//for(set<Rule>::iterator rit = x.rules.begin(); rit != x.rules.end(); ++rit)
		//	if(*rit != node->getDifference()) ast.rules.insert(*rit);

		OrderTypes::const_iterator otit = x.ordertypes.begin();
		for(Order::const_iterator oit = x.order.begin(); oit != x.order.end(); ++oit)
		{
			if(*oit == node->getDifference() && *otit)
			{
				++otit;
				continue;
			}

			ast.order.push_back(*oit);
			ast.ordertypes.push_back(*otit++);
		}

		for(map<Variable, set<Rule> >::iterator git = x.guards.begin();
			git != x.guards.end();
			++git)
		{
			if(git->second.find(node->getDifference()) != git->second.end())
				continue;

			ast.guards.insert(*git);
		}

		for(set<set<Rule> >::iterator git = x.guardsdown.begin();
			git != x.guardsdown.end();
			++git)
		{
			if(git->find(node->getDifference()) != git->end())
				continue;

			ast.guardsdown.insert(*git);
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

