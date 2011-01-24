#include <iostream>
#include <algorithm>

#include "HCFAnswerSet.h"

using namespace std;

#if defined(VERBOSE) && defined(DEBUG)
static void printTuples(TupleSet *svals, const ExtendedHypertree *node)
{
        cout << "SVALS: " << svals->size() << " for node " << node << endl;
        for(TupleSet::iterator it = svals->begin(); it != svals->end(); ++it)
        {
		HCFAnswerSetTuple &c = *(HCFAnswerSetTuple *)it->first;
		cout << "\tgraph: ";
		for(Graph::const_iterator i = c.graph.begin(); i != c.graph.end(); ++i)
		{
			cout << "[" << i->first << " > "; printIntSet(i->second); cout << "]";
		}
		cout << ", sat: "; printIntSet(c.satisfied);
		cout << endl;
        }
}
#endif

HCFAnswerSetTuple::HCFAnswerSetTuple() 
{
	// add the default DERIVED vertex to the graph
	this->addVertex(DERIVED);
}

HCFAnswerSetTuple::HCFAnswerSetTuple(HCFAnswerSetTuple &o) 
{
	this->graph = o.graph;
	this->reversegraph = o.reversegraph;
	this->satisfied = o.satisfied;
}

HCFAnswerSetTuple::~HCFAnswerSetTuple() { }

bool HCFAnswerSetTuple::operator<(const Tuple &other) const
{
	HCFAnswerSetTuple &o = (HCFAnswerSetTuple &)other;

	return     (this->graph < o.graph
		|| (this->graph == o.graph
		&&  this->satisfied < o.satisfied));
}

bool HCFAnswerSetTuple::operator==(const Tuple &other) const
{
	HCFAnswerSetTuple &o = (HCFAnswerSetTuple &)other;

	return 	   this->graph == o.graph
		&& this->satisfied == o.satisfied;
}

int HCFAnswerSetTuple::hash() const
{
	//TODO
	return -1;
}

void HCFAnswerSetTuple::addVertex(Vertex v)
{
	this->graph[v];
	this->reversegraph[v];
}

void HCFAnswerSetTuple::removeVertex(Vertex v)
{
	for(VertexSet::iterator it = this->graph[v].begin(); it != this->graph[v].end(); ++it)
		this->reversegraph[*it].erase(v);
	for(VertexSet::iterator it = this->reversegraph[v].begin(); it != this->reversegraph[v].end(); ++it)
		this->graph[*it].erase(v);

	this->graph.erase(v);
	this->reversegraph.erase(v);
}

void HCFAnswerSetTuple::addEdge(Vertex v1, Vertex v2)
{
	if(v1 == v2) return;

	Graph::iterator it1 = this->graph.find(v1);
	Graph::iterator it2 = this->graph.find(v2);

	CNOT0(it1 == this->graph.end());
	CNOT0(it2 == this->graph.end());

	it1->second.insert(v2);
	this->reversegraph[v2].insert(v1);
}

void HCFAnswerSetTuple::transitiveClosureThrough(Vertex v)
{
	VertexSet ancestors, successors;

	// accumulate all ancestors of v
	ancestors.insert(v);
	for(VertexSet::const_iterator a = this->reversegraph[v].begin();
			a != this->reversegraph[v].end(); ++a)
	{
		if(*a == DERIVED) continue;
		ancestors.insert(this->reversegraph[*a].begin(), this->reversegraph[*a].end());
		ancestors.insert(*a);
	}
	ancestors.erase(DERIVED);

	// accumulate all successors of v
	successors.insert(v);
	for(VertexSet::const_iterator s = this->graph[v].begin();
			s != this->graph[v].end(); ++s)
	{
		if(*s == DERIVED) continue;
		successors.insert(this->graph[*s].begin(), this->graph[*s].end());
		successors.insert(*s);
	}
	successors.erase(DERIVED);
	
	// connect every ancestor to all successors
	for(VertexSet::const_iterator a = ancestors.begin();
			a != ancestors.end(); ++a)
		for(VertexSet::const_iterator s = successors.begin();
				s != successors.end(); ++s)
			this->addEdge(*a, *s);
}

bool HCFAnswerSetTuple::hasCycleThrough(Vertex v)
{
	for(VertexSet::const_iterator s = this->graph[v].begin();
			s != this->graph[v].end(); ++s)
		if(this->graph[*s].find(v) != this->graph[*s].end())
			return true;
	return false;
}

HCFAnswerSetAlgorithm::HCFAnswerSetAlgorithm(Problem *problem)
	: AnswerSetAlgorithm(problem)
{
}

HCFAnswerSetAlgorithm::~HCFAnswerSetAlgorithm() { }

Solution *HCFAnswerSetAlgorithm::selectSolution(TupleSet *tuples, const ExtendedHypertree *root)
{
	Solution *s = this->instantiator->createEmptySolution();

	RuleSet rules = getRules(root);
	VariableSet variables = getVariables(root);

	for(TupleSet::iterator it = tuples->begin(); it != tuples->end(); ++it)
	{
		HCFAnswerSetTuple &x = *(HCFAnswerSetTuple *)it->first;

		bool filter = false;
		for(RuleSet::const_iterator rit = rules.begin(); rit != rules.end(); ++rit)
			if(x.reversegraph[DERIVED].find(*rit) == x.reversegraph[DERIVED].end()
				&& x.satisfied.find(*rit) == x.satisfied.end())
			{
				filter = true;
				break;
			}
		if(filter) continue;

		for(VariableSet::const_iterator vit = variables.begin(); vit != variables.end(); ++vit)
			if(x.graph.find(*vit) != x.graph.end()
				&& x.graph[DERIVED].find(*vit) == x.graph[DERIVED].end())
			{
				filter = true;
				break;
			}
		if(filter) continue;

		s = this->instantiator->combine(Union, s, it->second);
	}

	return s;

}

TupleSet *HCFAnswerSetAlgorithm::evaluateLeafNode(const ExtendedHypertree *node)
{
	TupleSet *ts = new TupleSet();

	VariableSet nodeVariables = getVariables(node);
	RuleSet nodeRules = getRules(node);
	VertexSet vertices = node->getVertices();

	Partition apart = Helper::partition(vertices);

        for(unsigned int i = 0; i < apart.first.size(); ++i)
	{
		VariableSet positiveVariables;
		set_intersection(apart.first[i].begin(), apart.first[i].end(),
				nodeVariables.begin(), nodeVariables.end(),
				inserter(positiveVariables, positiveVariables.begin()));

		HCFAnswerSetTuple *x = new HCFAnswerSetTuple(), *t = NULL;

		for(VertexSet::const_iterator it = apart.first[i].begin();
				it != apart.first[i].end(); ++it)
		{
			if(nodeRules.find(*it) != nodeRules.end()) t = copyTupleAddProofRule(*x, nodeVariables, *it);
			else t = copyTupleAddPositiveVariable(*x, nodeRules, *it);

			delete x; x = t;
			if(!x) break;
		}
		if(!x) continue;

		for(VertexSet::const_iterator it = apart.second[i].begin();
				it != apart.second[i].end(); ++it)
		{
			if(nodeRules.find(*it) != nodeRules.end()) t = copyTupleAddNonProofRule(*x, nodeVariables, *it);
			else t = copyTupleAddNegativeVariable(*x, nodeRules, *it);

			delete x; x = t;
			if(!x) break;
		}
		if(!x) continue;

		ts->insert(TupleSet::value_type(x,
			this->instantiator->createLeafSolution(positiveVariables)));
	}

#if defined(VERBOSE) && defined(DEBUG)
	printTuples(ts, node);
#endif

        return ts;
}

TupleSet *HCFAnswerSetAlgorithm::evaluateBranchNode(const ExtendedHypertree *node)
{
	TupleSet *left = this->evaluateNode(node->firstChild()), 
		*right = this->evaluateNode(node->secondChild());
	TupleSet *ts = new TupleSet();

	RuleSet nodeRules = getRules(node);

	for(TupleSet::iterator lit = left->begin(); lit != left->end(); ++lit)
	{
		// the tuple from the left
		HCFAnswerSetTuple &l = *(HCFAnswerSetTuple *)lit->first;
		for(TupleSet::iterator rit = right->begin(); rit != right->end(); ++rit)
		{
			// the tuple from the right
			HCFAnswerSetTuple &r = *(HCFAnswerSetTuple *)rit->first;
			
			// try to combine the tuples (left/right)
			HCFAnswerSetTuple *t = this->combineTuples(l, r, nodeRules);
			if(t)
			{
				Solution *s = this->instantiator->combine(CrossJoin, 
						lit->second, rit->second);
				this->addToTupleSet(*t, s, ts);
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

TupleSet *HCFAnswerSetAlgorithm::evaluateVariableIntroductionNode(const ExtendedHypertree *node)
{
	TupleSet *base = this->evaluateNode(node->firstChild());
	TupleSet *ts = new TupleSet();

	RuleSet nodeRules = getRules(node);

	for(TupleSet::iterator it = base->begin(); it != base->end(); ++it)
	{
		// the old tuple
		HCFAnswerSetTuple &x = *(HCFAnswerSetTuple *)it->first;

		// try to insert the variable into the tuple (set to true)
		HCFAnswerSetTuple *t = this->copyTupleAddPositiveVariable(x, nodeRules, node->getDifference());
		if(t)
		{
			Solution *s = this->instantiator->addDifference(it->second, node->getDifference());
			this->addToTupleSet(*t, s, ts);
		}

		// try to insert the variable into the tuple (set to false)
		t = this->copyTupleAddNegativeVariable(x, nodeRules, node->getDifference());
		if(t) this->addToTupleSet(*t, it->second, ts);
	}

	delete base;

#if defined(VERBOSE) && defined(DEBUG)
	printTuples(ts, node);
#endif

	return ts;
}

TupleSet *HCFAnswerSetAlgorithm::evaluateVariableRemovalNode(const ExtendedHypertree *node)
{
	TupleSet *base = this->evaluateNode(node->firstChild());
	TupleSet *ts = new TupleSet();

	RuleSet nodeRules = getRules(node);

	for(TupleSet::iterator it = base->begin(); it != base->end(); ++it)
	{
		// the old tuple
		HCFAnswerSetTuple &x = *(HCFAnswerSetTuple *)it->first;

		// if the variable is in the graph but has not been derived, cancel operation
		if(x.graph.find(node->getDifference()) != x.graph.end()
			&& x.graph[DERIVED].find(node->getDifference()) == x.graph[DERIVED].end())
			continue;

		// create a copy of the tuple
		HCFAnswerSetTuple &t = *new HCFAnswerSetTuple(x);
		
		// remove the rule from the tuple
		t.removeVertex(node->getDifference());

		// add the tuple to the set with Union on duplicate
		this->addToTupleSet(t, it->second, ts);
	}

	delete base;

#if defined(VERBOSE) && defined(DEBUG)
	printTuples(ts, node);
#endif

	return ts;
}

TupleSet *HCFAnswerSetAlgorithm::evaluateRuleIntroductionNode(const ExtendedHypertree *node)
{
	TupleSet *base = this->evaluateNode(node->firstChild());
	TupleSet *ts = new TupleSet();

	VariableSet nodeVariables = getVariables(node);

	for(TupleSet::iterator it = base->begin(); it != base->end(); ++it)
	{
		// the old tuple
		HCFAnswerSetTuple &x = *(HCFAnswerSetTuple *)it->first;

		// try to insert the rule into the tuple as proof rule
		HCFAnswerSetTuple *t = this->copyTupleAddProofRule(x, nodeVariables, node->getDifference());
		if(t) this->addToTupleSet(*t, it->second, ts);

		// try to insert the rule into the tuple as non-proof rule
		t = this->copyTupleAddNonProofRule(x, nodeVariables, node->getDifference());
		if(t) this->addToTupleSet(*t, it->second, ts);
	}

	delete base;

#if defined(VERBOSE) && defined(DEBUG)
	printTuples(ts, node);
#endif

	return ts;
}

TupleSet *HCFAnswerSetAlgorithm::evaluateRuleRemovalNode(const ExtendedHypertree *node)
{
	TupleSet *base = this->evaluateNode(node->firstChild());
	TupleSet *ts = new TupleSet();

	for(TupleSet::iterator it = base->begin(); it != base->end(); ++it)
	{
		// the old tuple
		HCFAnswerSetTuple &x = *(HCFAnswerSetTuple *)it->first;

		Graph::const_iterator git;
		if((git = x.graph.find(node->getDifference())) == x.graph.end())
		{
			// if the rule is not in the graph and not satisfied, cancel operation
			if(x.satisfied.find(node->getDifference()) == x.satisfied.end())
				continue;
		}
		else
		{
			// if the rule is in the graph but not derived yet, cancel operation
			if(git->second.find(DERIVED) == git->second.end())
				continue;
		}

		// create a copy of the tuple
		HCFAnswerSetTuple &t = *new HCFAnswerSetTuple(x);

		// remove the rule from the tuple
		t.removeVertex(node->getDifference());
		t.satisfied.erase(node->getDifference());

		// add the tuple to the set with Union on duplicate
		this->addToTupleSet(t, it->second, ts);
	}

	delete base;

#if defined(VERBOSE) && defined(DEBUG)
	printTuples(ts, node);
#endif

	return ts;
}

void HCFAnswerSetAlgorithm::addToTupleSet(Tuple &t, Solution *s, TupleSet *ts, Operation op)
{
	// try to insert the tuple into the tuple set
	pair<TupleSet::iterator, bool> result = ts->insert(TupleSet::value_type(&t, s));

	// if the tuple was already in the set
	if(!result.second)
	{
		// delete it and insert it again with combined solution
		Solution *orig = result.first->second;
		ts->erase(result.first); 
		ts->insert(TupleSet::value_type(&t, this->instantiator->combine(op, orig, s)));
	}
}

HCFAnswerSetTuple *HCFAnswerSetAlgorithm::combineTuples(HCFAnswerSetTuple &l, HCFAnswerSetTuple &r, RuleSet rules)
{
	Graph::const_iterator lit = l.graph.begin(),
			      rit = r.graph.begin();

	// check if the graph vertices coincide, otherwise abort
	while(lit != l.graph.end() && rit != r.graph.end())
		if(lit->first != rit->first) return NULL;
		else { ++lit; ++rit; }
	if(lit != l.graph.end() || rit != r.graph.end()) return NULL;

	VertexSet::const_iterator ldit = l.reversegraph[DERIVED].begin(),
				  rdit = r.reversegraph[DERIVED].begin();

	while(ldit != l.reversegraph[DERIVED].end() && rdit != r.reversegraph[DERIVED].end())
		if(*ldit < *rdit) ++ldit;
		else if(*ldit > *rdit) ++rdit;
		else // if the same rule is derived left and right
		{
			// check that a head variable exists in both graphs
			bool inbothgraphs = false;
			for(VariableSet::const_iterator it = this->problem->getHeadMap()[*ldit].begin();
				it != this->problem->getHeadMap()[*ldit].end(); ++it)
			{
				if(l.graph[*ldit].find(*it) != l.graph[*ldit].end()
					&& r.graph[*rdit].find(*it) != r.graph[*rdit].end())
				{
					inbothgraphs = true;
					break;
				}
			}

			// if not, then abort
			if(!inbothgraphs) return NULL;

			// otherwise continue
			++ldit; ++rdit;
		}

	// now that all the preconditions are met, start combining the tuples
	HCFAnswerSetTuple *t = new HCFAnswerSetTuple(l); // copy the left tuple

	// add all edges from the right tuple to the left and check consistency
	for(rit = r.graph.begin(); rit != r.graph.end(); ++rit)
	{
		for(rdit = r.graph[rit->first].begin(); rdit != r.graph[rit->first].end(); ++rdit)
		{
			t->addEdge(rit->first, *rdit);
		}

		if(rit->first != DERIVED)
		{
			t->transitiveClosureThrough(rit->first); // calculate the transitive closure over the graph
			if(t->hasCycleThrough(rit->first)) { delete t; return NULL; } // check for inconsistency
		}
	}

	// combine the satisfied rules
	t->satisfied.insert(r.satisfied.begin(), r.satisfied.end());

	return t;
}

HCFAnswerSetTuple *HCFAnswerSetAlgorithm::copyTupleAddPositiveVariable(HCFAnswerSetTuple &x, RuleSet rules, Variable v)
{
	HCFAnswerSetTuple *t = new HCFAnswerSetTuple(x); // copy the original tuple
	t->addVertex(v);

	for(RuleSet::const_iterator it = rules.begin(); it != rules.end(); ++it)
	{
		//map<Variable, bool>::const_iterator sit;

		bool ingraph = x.graph.find(*it) != x.graph.end();
		//bool inhead = this->problem->getHeadMap()[*it].find(v) != this->problem->getHeadMap()[*it].end();
		bool inhead = this->isInRuleHead(*it, v);
		//bool inrule = (sit = this->problem->getSignMap()[*it].find(v)) != this->problem->getSignMap()[*it].end();
		bool inrule = this->isInRuleBody(*it, v);
		//bool inposbody = inrule && sit->second;
		bool inposbody = inrule && !this->isInNegativeRuleBody(*it, v);
		//bool innegbody = inrule && !sit->second && !inhead;
		bool innegbody = inrule && !inposbody && !inhead;
		bool isderived = ingraph && x.graph[*it].find(DERIVED) != x.graph[*it].end();
		
		if(ingraph)
		{
			if(innegbody) { delete t; return NULL; } // if the rule is in the graph but v is in the neg. body, abort
			if(inposbody) t->addEdge(v, *it); // if instead in positive body, add edge

			if(inhead)
			{
				if(isderived) { delete t; return NULL; } // if v is in the head but rule already derived, abort
				else 
				{
					// if not yet derived, add appropriate edges
					t->addEdge(*it, v);
					t->addEdge(*it, DERIVED);
					t->addEdge(DERIVED, v);
				}
			}
		}
		else if(innegbody || inhead) t->satisfied.insert(*it); // if rule is not in graph but satisfied, mark it
	}

	t->transitiveClosureThrough(v); // calculate the transitive closure over the graph
	if(t->hasCycleThrough(v)) { delete t; return NULL; } // if the resulting graph has a cycle, abort
	return t;
}

HCFAnswerSetTuple *HCFAnswerSetAlgorithm::copyTupleAddNegativeVariable(HCFAnswerSetTuple &x, RuleSet rules, Variable v)
{
	HCFAnswerSetTuple *t = new HCFAnswerSetTuple(x); // copy the original tuple

	for(RuleSet::const_iterator it = rules.begin(); it != rules.end(); ++it)
	{
		// check if the variable is contained in some rule
		//map<Variable, bool>::const_iterator sit;
		//if((sit = this->problem->getSignMap()[*it].find(v)) != this->problem->getSignMap()[*it].end())
		if(this->isInRuleBody(*it, v))
		{
			//
			bool inposbody = !this->isInNegativeRuleBody(*it, v);
			if(x.graph.find(*it) != x.graph.end()) // if rule is in the graph (i.e. a proof rule)
			{
				//if(sit->second) { delete t; return NULL; } // abort if v is in positive body
				if(inposbody) { delete t; return NULL; } // abort if v is in positive body
			}
			//else if(sit->second) // if rule is not in the graph and v occurs in pos. body
			else if(inposbody) // if rule is not in the graph and v occurs in pos. body
			{
				t->satisfied.insert(*it); // set rule to satisfied
			}
		}
	}

	return t;
}

HCFAnswerSetTuple *HCFAnswerSetAlgorithm::copyTupleAddProofRule(HCFAnswerSetTuple &x, VariableSet variables, Rule r)
{
	HCFAnswerSetTuple *t = new HCFAnswerSetTuple(x); // copy the original tuple
	t->addVertex(r); 

	int headcount = 0;
	for(VariableSet::const_iterator it = variables.begin(); it != variables.end(); ++it)
	{
		//map<Variable, bool>::const_iterator sit;

		bool ingraph = x.graph.find(*it) != x.graph.end();
		//bool inhead = this->problem->getHeadMap()[r].find(*it) != this->problem->getHeadMap()[r].end();
		bool inhead = this->isInRuleHead(r, *it);
		//bool inrule = (sit = this->problem->getSignMap()[r].find(*it)) != this->problem->getSignMap()[r].end();
		bool inrule = this->isInRuleBody(r, *it);
		//bool inposbody = inrule && sit->second;
		bool inposbody = inrule && !this->isInNegativeRuleBody(r, *it);
		//bool innegbody = inrule && !sit->second && !inhead;
		bool innegbody = inrule && !inposbody && !inhead;
	
		if(ingraph && inhead) // if we found a head variable
		{
			if(++headcount > 1) { delete t; return NULL; } // if it is the second one, abort
			else
			{
				// otherwise insert edges accordingly
				t->addEdge(r, *it);
				t->addEdge(r, DERIVED);
				t->addEdge(DERIVED, *it);
			}
		}

		
		if(inrule) // if some variable is containted in the rule
		{
			if((ingraph && innegbody) || (!ingraph && inposbody)) { delete t; return NULL; } // and doesn't satisfy it, abort
			if(ingraph && inposbody) t->addEdge(*it, r); // if it does satisfy the rule, add an edge
		}
	}
	
	t->transitiveClosureThrough(r); // calculate the transitive closure over the graph
	if(t->hasCycleThrough(r)) { delete t; return NULL; } // if the resulting graph has a cycle, abort
	return t;
}

HCFAnswerSetTuple *HCFAnswerSetAlgorithm::copyTupleAddNonProofRule(HCFAnswerSetTuple &x, VariableSet variables, Rule r)
{
	HCFAnswerSetTuple *t = new HCFAnswerSetTuple(x); // copy the original tuple

	for(VariableSet::const_iterator it = variables.begin(); it != variables.end(); ++it)
	{
		//map<Variable, bool>::const_iterator sit;
		//if((sit = this->problem->getSignMap()[r].find(*it)) != this->problem->getSignMap()[r].end() // if some variable is containted in the rule
		//		&& !((x.graph.find(*it) != x.graph.end()) ^ !sit->second)) // and it satisfies the rule
		if(this->isInRuleBody(r, *it) && !((x.graph.find(*it) != x.graph.end()) ^ this->isInNegativeRuleBody(r, *it)))
		{
			t->satisfied.insert(r); // set it to satisfied
			break;
		}
	}

	return t;
}

