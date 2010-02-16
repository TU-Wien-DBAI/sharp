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
					trueRules(apart.first[i],
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

			OrderCombinations combinations = combineOrder(l.order, l.ordertypes,
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
			astf.guards[git->first] = trueRules(	astf.variables,
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
			set<Rule> next = trueRules(	astf.variables,
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
			combineOrder(x.order, x.ordertypes, node->getDifference(), false);

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
				astt.guards[git->first] = trueRules(	astt.variables,
									node->getVariables(),
									git->second,
									git->first,
									astt.order,
									astt.ordertypes,
									this->getSignMap(),
									this->getHeadMap());
			}

			set<Rule> diffg, truediffg;

			truediffg =  trueRules(	astt.variables,
						node->getVariables(),
						node->getRules(),
						node->getDifference(),
						astt.order,
						astt.ordertypes,
						this->getSignMap(),
						this->getHeadMap());

			set_difference(truediffg.begin(), truediffg.end(), x.rules.begin(), x.rules.end(),
						inserter(diffg, diffg.begin()));

			astt.guards[node->getDifference()] = diffg;
	
			for(set<set<Rule> >::iterator git = x.guardsdown.begin();
				git != x.guardsdown.end();
				++git)
			{
				set<Rule> next = trueRules(	astt.variables,
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

		for(set<Rule>::iterator rit = node->getRules().begin(); rit != node->getRules().end(); ++rit)
		{
			map<Variable, bool>::iterator rulesigns;
			if((rulesigns = this->signMap[*rit].find(node->getDifference())) 
				!= this->signMap[*rit].end())
			{
				if(rulesigns->second) l1.insert(*rit);
			}
		}

		OrderCombinations combinations =
			combineOrder(x.order, x.ordertypes, l1, l2, node->getDifference(), false);

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
			if(trueRule(ast.variables,
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

/********************************\
| INTERNAL HELPER methods
\********************************/

//FIXME: this is veeeeery inefficient because list is copied in every recursion. use pointers?
static OrderCombinations internal_combine(	Order::const_iterator l,
						Order::const_iterator lend,
						OrderTypes::const_iterator lt,
						OrderTypes::const_iterator ltend,
						Order::const_iterator r,
						Order::const_iterator rend,
						OrderTypes::const_iterator rt,
						OrderTypes::const_iterator rtend)
{
	OrderCombinations combinations;
	
	if(l == lend) combinations.push_back(make_pair(Order(r, rend), OrderTypes(rt, rtend)));
	else if(r == rend) combinations.push_back(make_pair(Order(l, lend), OrderTypes(lt, ltend)));
	else
	{
		Order::const_iterator add = l;
		OrderTypes::const_iterator addtype = lt;

		OrderCombinations temp = internal_combine(++l, lend, ++lt, ltend, r, rend, rt, rtend);
		for(OrderCombinations::iterator it = temp.begin(); it != temp.end(); ++it)
		{ it->first.push_front(*add); it->second.push_front(*addtype); }
		combinations.splice(combinations.end(), temp);

		l = add; add = r; lt = addtype; addtype = rt;

		temp = internal_combine(l, lend, lt, ltend, ++r, rend, ++rt, rtend);
		for(OrderCombinations::iterator it = temp.begin(); it != temp.end(); ++it)
		{ it->first.push_front(*add); it->second.push_front(*addtype); }
		combinations.splice(combinations.end(), temp);
	}

	return combinations;
}

/**************************************\
| HELPER Methods
\**************************************/

set<Rule> HeadCycleFreeAnswerSetAlgorithm::trueRules(	const set<Variable> &positives,
							const set<Variable> &all,
							const set<Rule> &rules,
							Variable variable,
							const Order &order,
							const OrderTypes &ordertypes,	
							const SignMap &signs,
							const HeadMap &heads)
{	
	//TODO
	set<Rule> truerules;
        set<Variable> negatives;
        set<Variable> intersect;

        set_difference(all.begin(), all.end(), positives.begin(), positives.end(),
                                        inserter(negatives, negatives.begin()));

	cout << "trueRules(" << variable << "): ";
        cout << "pos: "; printIntSet(positives);
        cout << ", neg: "; printIntSet(negatives);
	cout << ", rul: "; printIntSet(rules);
	cout << ", ord: "; printIntList(order);
	cout << ", typ: "; printBoolList(ordertypes); cout<<endl;

        for(set<Rule>::const_iterator r = rules.begin(); r != rules.end(); ++r)
        {
		intersect.clear();

                //FIXME danger: linear in the size of the head of the rule... how to??
                set_intersection(heads[*r].begin(), heads[*r].end(),
                                positives.begin(), positives.end(),
                                inserter(intersect, intersect.begin()));
                bool add = (intersect.size() == 1 && *intersect.begin() == variable);cout<<"Rule "<< *r << ": "; printIntSet(heads[*r]); cout<<" - "; printIntSet(intersect); cout<<" - "<<add;

                SignMap::const_iterator posneg = signs.find(*r);

                //FIXME slow, utilize the fact that sets are sorted (multiple finds -> log(n))
                //FIXME simplification possible?
                for(set<Variable>::const_iterator var = positives.begin();
                        add && var != positives.end(); ++var)
                {
                        map<Variable, bool>::const_iterator it = posneg->second.find(*var);
			set<Variable>::const_iterator hit = heads[*r].find(*var);
                        add = hit != heads[*r].end() || it == posneg->second.end() || it->second;
                }cout<<add;
                for(set<Variable>::const_iterator var = negatives.begin();
                        add && var != negatives.end(); ++var)
                {
                        map<Variable, bool>::const_iterator it = posneg->second.find(*var);
                        add = it == posneg->second.end() || !it->second;
                }cout<<add;

                bool seen = false;
                OrderTypes::const_iterator otit = ordertypes.begin();
		for(Order::const_iterator oit = order.begin(); add && oit != order.end(); ++oit)
		{
			OrderTypes::const_iterator currtype = otit++;
			if(!seen && !*currtype && *oit == variable) { seen = true; continue; }
			else if(!seen) continue;
			else if(!*currtype)
			{
				map<Variable, bool>::const_iterator it = posneg->second.find(*oit);
				add = it == posneg->second.end() || !it->second;
			}
			else if(*oit == *r) { add = false; }
		}cout<<add<<endl;
		
		if(add) truerules.insert(*r);
	}

	return truerules;
}

set<Rule> HeadCycleFreeAnswerSetAlgorithm::trueRules(	const set<Variable> &positives,
							const set<Variable> &all,
							const set<Rule> &rules,
							const Order &order,
							const OrderTypes &ordertypes,
							const SignMap &signs,
							const HeadMap &heads)
{
	//TODO
	set<Rule> truerules;
        set<Variable> negatives;
        set<Variable> intersect;

        set_difference(all.begin(), all.end(), positives.begin(), positives.end(),
                                        inserter(negatives, negatives.begin()));

        for(set<Rule>::const_iterator r = rules.begin(); r != rules.end(); ++r)
        {
                //FIXME danger: linear in the size of the head of the rule... how to??
                set_intersection(heads[*r].begin(), heads[*r].end(),
                                positives.begin(), positives.end(),
                                inserter(intersect, intersect.begin()));
                bool add = (intersect.size() == 0);

                SignMap::const_iterator posneg = signs.find(*r);

                //FIXME slow, utilize the fact that sets are sorted (multiple finds -> log(n))
                //FIXME simplification possible?
                for(set<Variable>::const_iterator var = positives.begin();
                        add && var != positives.end(); ++var)
                {
			map<Variable, bool>::const_iterator it = posneg->second.find(*var);
			set<Variable>::const_iterator hit = heads[*r].find(*var);
                        add = hit != heads[*r].end() || it == posneg->second.end() || it->second;
                }
                for(set<Variable>::const_iterator var = negatives.begin();
                        add && var != negatives.end(); ++var)
                {
                        map<Variable, bool>::const_iterator it = posneg->second.find(*var);
                        add = it == posneg->second.end() || !it->second;
                }

                bool seen = false;
                OrderTypes::const_iterator otit = ordertypes.begin();
                for(Order::const_iterator oit = order.begin(); add && oit != order.end(); ++oit)
                {
                        OrderTypes::const_iterator currtype = otit++;
                        if(!seen && *currtype && *oit == *r) { seen = true; continue; }
                        else if(!seen) continue;
                        else if(!*currtype)
                        {
                                map<Variable, bool>::const_iterator it = posneg->second.find(*oit);
				add = it == posneg->second.end() || !it->second;
                        }
		}
		
		if(add) truerules.insert(*r);
	}

	return truerules;
}

bool HeadCycleFreeAnswerSetAlgorithm::trueRule(	const set<Variable> &positives,
						const set<Variable> &all,
						Rule rule,
						Variable variable,
						const Order &order,
						const OrderTypes &ordertypes,
						const SignMap &signs,
						const HeadMap &heads)
{
	set<Rule> rules; rules.insert(rule);
	return trueRules(positives, all, rules, variable, order, ordertypes, signs, heads).size() != 0;
}

OrderCombinations HeadCycleFreeAnswerSetAlgorithm::combineOrder(const Order &original,
								const OrderTypes &types,
								const set<Rule> &left,
								const set<Rule> &right,
								int separator,
								bool separatorType)
{
	//TODO
	OrderCombinations leftcomb, rightcomb, leftsingle, rightsingle, combinations;
	Order leftinit, rightinit;
	OrderTypes lefttypes, righttypes;

	Order::const_iterator oit = original.begin();
	OrderTypes::const_iterator tit = types.begin();

	set<Rule> lset(left), rset(right);

	bool sepfound = false;
	if(original.size() > 0)
	{
		while(oit != original.end() && !(*oit == separator && *tit == separatorType))
		{ 
			leftinit.push_back(*oit++); lefttypes.push_back(*tit++); 
			if(*tit) { lset.erase(leftinit.back()); rset.erase(leftinit.back()); }
		}

		if(oit != original.end())
		{
			sepfound = true;
			while(++oit != original.end() && ++tit != types.end()) 
			{ 
				rightinit.push_back(*oit); righttypes.push_back(*tit); 
				if(*tit) { lset.erase(rightinit.back()); rset.erase(rightinit.back()); }
			}
		}
	}
	
	leftcomb.push_back(make_pair(leftinit, lefttypes));
	rightcomb.push_back(make_pair(rightinit, righttypes));

	if(!sepfound)
	{
		leftsingle.push_back(make_pair(Order(), OrderTypes()));
		rightsingle.push_back(make_pair(Order(), OrderTypes()));
	}

	//left loop
	for(set<Rule>::const_iterator lit = lset.begin(); lit != lset.end(); ++lit)
	{
		OrderCombinations temp;
		for(OrderCombinations::iterator it = leftcomb.begin(); it != leftcomb.end(); ++it)
		{
			OrderCombinations noref = combineOrder(it->first, it->second, *lit, true);
			temp.splice(temp.end(), noref);
		}
		leftcomb.swap(temp);
	}

	//right loop
	for(set<Rule>::const_iterator rit = rset.begin(); rit != rset.end(); ++rit)
	{
		OrderCombinations temp;
		for(OrderCombinations::iterator it = rightcomb.begin(); it != rightcomb.end(); ++it)
		{
			OrderCombinations noref = combineOrder(it->first, it->second, *rit, true);
			temp.splice(temp.end(), noref);
		}
		rightcomb.swap(temp);
	}

	//loop again without external variables
	if(!sepfound)
	{
		//left loop
        	for(set<Rule>::const_iterator lit = lset.begin(); lit != lset.end(); ++lit)
        	{
                	OrderCombinations temp;
                	for(OrderCombinations::iterator it = leftsingle.begin(); it != leftsingle.end(); ++it)
                	{
       	        	        OrderCombinations noref = combineOrder(it->first, it->second, *lit, true);
       	        	        temp.splice(temp.end(), noref);
       	        	}
       	        	leftsingle.swap(temp);
	        }
	
	        //right loop
	        for(set<Rule>::const_iterator rit = rset.begin(); rit != rset.end(); ++rit)
        	{
                	OrderCombinations temp;
                	for(OrderCombinations::iterator it = rightsingle.begin(); it != rightsingle.end(); ++it)
                	{
                        	OrderCombinations noref = combineOrder(it->first, it->second, *rit, true);
                        	temp.splice(temp.end(), noref);
               		}
               		rightsingle.swap(temp);
        	}
	}
	
	//merge loops
	if(sepfound)
		for(OrderCombinations::iterator lit = leftcomb.begin(); lit != leftcomb.end(); ++lit)
			for(OrderCombinations::iterator rit = rightcomb.begin(); rit != rightcomb.end(); ++rit)
			{
				Order neworder(lit->first); 
				neworder.insert(neworder.end(), rit->first.begin(), rit->first.end());
				OrderTypes newtypes(lit->second);
				newtypes.insert(newtypes.end(), rit->second.begin(), rit->second.end());
	
				combinations.push_back(make_pair(neworder, newtypes));
			}
	else
	{
		for(OrderCombinations::iterator lit = leftcomb.begin(); lit != leftcomb.end(); ++lit)
			for(OrderCombinations::iterator rit = rightsingle.begin(); rit != rightsingle.end(); ++rit)
			{
				Order neworder(lit->first); 
				neworder.insert(neworder.end(), rit->first.begin(), rit->first.end());
				OrderTypes newtypes(lit->second);
				newtypes.insert(newtypes.end(), rit->second.begin(), rit->second.end());
	
				combinations.push_back(make_pair(neworder, newtypes));
			}

		
		for(OrderCombinations::iterator lit = leftsingle.begin(); lit != leftsingle.end(); ++lit)
			for(OrderCombinations::iterator rit = rightcomb.begin(); rit != rightcomb.end(); ++rit)
			{
				Order neworder(lit->first); 
				neworder.insert(neworder.end(), rit->first.begin(), rit->first.end());
				OrderTypes newtypes(lit->second);
				newtypes.insert(newtypes.end(), rit->second.begin(), rit->second.end());
	
				combinations.push_back(make_pair(neworder, newtypes));
			}

		combinations.unique();
	}

	return combinations;
}

OrderCombinations HeadCycleFreeAnswerSetAlgorithm::combineOrder(const Order &original,
								const OrderTypes &types,
								int toInsert,
								bool insertType)
{
	//TODO
	OrderCombinations combinations;
	Order::const_iterator oit = original.begin();
	OrderTypes::const_iterator tit = types.begin();

	do
	{
		if(*oit == toInsert && *tit == insertType) 
			return OrderCombinations(1, make_pair(original, types));

		Order neworder(original.begin(), oit);
		OrderTypes newtypes(types.begin(), tit);

		neworder.push_back(toInsert);
		newtypes.push_back(insertType);

		neworder.insert(neworder.end(), oit, original.end());
		newtypes.insert(newtypes.end(), tit, types.end());

		combinations.push_back(make_pair(neworder, newtypes));

		if(oit == original.end()) break;
		++oit; ++tit;
	}
	while(true);

	return combinations;
}						

OrderCombinations HeadCycleFreeAnswerSetAlgorithm::combineOrder(const Order &left,
								const OrderTypes &lefttypes,
								const Order &right,
								const OrderTypes &righttypes)
{
	//TODO
	OrderCombinations combinations;
	list<OrderCombinations> partials;

	int currpos, maxpos = 0;
	Order::const_iterator locurr = left.begin(), rocurr = right.begin();
	OrderTypes::const_iterator ltcurr = lefttypes.begin(), rtcurr = righttypes.begin();

	OrderTypes::const_iterator ltit = ltcurr;
	for(Order::const_iterator loit = locurr; loit != left.end(); ++loit)
	{
		currpos = 0;
		OrderTypes::const_iterator rtit = righttypes.begin();
		for(Order::const_iterator roit = right.begin(); roit != right.end(); ++roit)
		{
			if(*ltit == *rtit && *roit == *loit) //matchpoint found
			{
				//left and right orders are inconsistent
				if(currpos < maxpos) return combinations;

				maxpos = currpos;

				partials.push_back(internal_combine(	locurr, loit,
									ltcurr, ltit,
									rocurr, roit,
									rtcurr, rtit));

				partials.push_back(OrderCombinations(1, 
					make_pair(Order(1, *loit), OrderTypes(1, *ltit))));

				locurr = loit; ++locurr;
				ltcurr = ltit; ++ltcurr;
				rocurr = roit; ++rocurr;
				rtcurr = rtit; ++rtcurr;
			}

			++currpos;
			++rtit;
		}
		
		++ltit;
	}

	partials.push_back(internal_combine(	locurr, left.end(),
						ltcurr, lefttypes.end(),
						rocurr, right.end(),
						rtcurr, righttypes.end()));

	combinations.swap(partials.front());
	partials.pop_front();

	while(!partials.empty())
	{
		OrderCombinations temp;
		for(OrderCombinations::iterator oit = combinations.begin(); 
			oit != combinations.end();
			++oit)
			for(OrderCombinations::iterator iit = partials.front().begin();
				iit != partials.front().end();
				++iit)
			{
				Order neworder(oit->first);
				neworder.insert(neworder.end(), iit->first.begin(), iit->first.end());
				OrderTypes newtypes(oit->second);
				newtypes.insert(newtypes.end(), iit->second.begin(), iit->second.end());
				temp.push_back(make_pair(neworder, newtypes));
			}

		combinations.swap(temp);
		partials.pop_front();
	}

	return combinations;
}
