#include <cmath>
#include <iostream>

#include <algorithm>

#include "Helper.h"

using namespace std;

Partition Helper::partition(const set<Variable> &variables)
{
	typedef set<Variable> set_t;
        typedef set_t::iterator iter_t;

	int size = int(pow(2.0f, (int)variables.size()));

        vector<set_t> positives, negatives; positives.reserve(size); negatives.reserve(size);
        vector<iter_t> elements;

        do
        {
                set_t newpos, newneg;
                vector<iter_t>::iterator vi = elements.begin();
                for(iter_t si = variables.begin(); si != variables.end(); ++si)
                        if(vi != elements.end() && si == *vi) { newpos.insert(*si); ++vi; }
                        else { newneg.insert(*si); }

                positives.push_back(newpos); negatives.push_back(newneg);

                if(!elements.empty() && ++elements.back() == variables.end()) elements.pop_back();
                else
                {
                        iter_t si;
                        if(elements.empty()) si = variables.begin(); else { si = elements.back(); ++si; }
                        for(; si != variables.end(); ++si) elements.push_back(si);
                }

        }
        while(!elements.empty());

        return Partition(positives, negatives);
}

set<Rule> Helper::trueRules(	const set<Variable> &positives, 
				const set<Variable> &negatives, 
				const set<Rule> &rules, 
				const SignMap &signs)
{

	//FIXME very slow, utilize the fact that sets are sorted
        set<int> truerules;

        for(set<int>::const_iterator r = rules.begin(); r != rules.end(); ++r)
        {
                bool add = false;
                for(set<int>::const_iterator var = positives.begin(); !add && var != positives.end(); ++var)
                {
			if(add) break;

                        SignMap::const_iterator posneg = signs.find(*r);
                        map<int, bool>::const_iterator it = posneg->second.find(*var);
                        add = it != posneg->second.end() && !it->second;
                }
                for(set<int>::const_iterator var = negatives.begin(); !add && var != negatives.end(); ++var)
                {
			if(add) break;

                        SignMap::const_iterator posneg = signs.find(*r);
                        map<int, bool>::const_iterator it = posneg->second.find(*var);
                        add = it != posneg->second.end() && it->second;
                }

                if(add) truerules.insert(*r);
        }

        return truerules;
}

set<Rule> Helper::trueRules(	const set<Variable> &base,
				const set<Variable> &positives, 
				const set<Variable> &negatives, 
				const set<Rule> &rules, 
				const SignMap &signs,
				const HeadMap &heads)
{
	//FIXME very slow, utilize the fact that sets are sorted
        set<Rule> truerules;

        for(set<Rule>::const_iterator r = rules.begin(); r != rules.end(); ++r)
        {
                bool add = false;
                for(set<Variable>::const_iterator var = positives.begin(); 
			!add && var != positives.end(); ++var)
                {
                        //SignMap::const_iterator posneg = signs.find(*r);
                        set<Variable>::const_iterator it = heads[*r].find(*var);
                        add = it != heads[*r].end();
                }
                for(set<Variable>::const_iterator var = negatives.begin(); 
			!add && var != negatives.end(); ++var)
                {
                        SignMap::const_iterator posneg = signs.find(*r);
                        map<Variable, bool>::const_iterator it = posneg->second.find(*var);
                        add = it != posneg->second.end() && it->second;
                }
		for(set<Variable>::const_iterator var = base.begin(); 
			!add && var != base.end(); ++var)
		{
			SignMap::const_iterator posneg = signs.find(*r);
			map<Variable, bool>::const_iterator pnit = posneg->second.find(*var);
			set<Variable>::const_iterator hit = heads[*r].find(*var);
			add = hit == heads[*r].end() 
				&& pnit != posneg->second.end() 
				&& !pnit->second;
		}

                if(add) truerules.insert(*r);
        }

	return truerules;
}

set<Rule> Helper::trueRules(	const set<Variable> &positives,
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
	
	for(set<Rule>::const_iterator r = rules.begin(); r != rules.end(); ++r)
	{
		//FIXME danger: linear in the size of the head of the rule... how to??
		intersect.clear();
		set_intersection(heads[*r].begin(), heads[*r].end(),
				positives.begin(), positives.end(),
				inserter(intersect, intersect.begin()));
		bool add = (intersect.size() == 1 && *intersect.begin() == variable);

		SignMap::const_iterator posneg = signs.find(*r);
		
		//FIXME slow, utilize the fact that sets are sorted (multiple finds -> log(n))
		//FIXME check if this is correct for this problem, simplification possible?
		for(set<Variable>::const_iterator var = positives.begin();
			add && var != positives.end(); ++var)
		{
			map<Variable, bool>::const_iterator it = posneg->second.find(*var);
			add = it == posneg->second.end() || !it->second; //FIXME: check for head?
		}
		for(set<Variable>::const_iterator var = negatives.begin(); 
			add && var != negatives.end(); ++var)
                {
                        map<Variable, bool>::const_iterator it = posneg->second.find(*var);
                        add = it == posneg->second.end() || it->second; //FIXME: check for head?
                }

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
				add = it == posneg->second.end() || it->second; //FIXME: check for head?
			}
			else if(*oit == *r) { add = false; }
		}
		
		if(add) truerules.insert(*r);
	}

	return truerules;
}

set<Rule> Helper::trueRules(	const set<Variable> &positives,
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
                //FIXME check if this is correct for this problem, simplification possible?
                for(set<Variable>::const_iterator var = positives.begin();
                        add && var != positives.end(); ++var)
                {
                        map<Variable, bool>::const_iterator it = posneg->second.find(*var);
                        add = it == posneg->second.end() || it->second; //FIXME: check for head?
                }
                for(set<Variable>::const_iterator var = negatives.begin();
                        add && var != negatives.end(); ++var)
                {
                        map<Variable, bool>::const_iterator it = posneg->second.find(*var);
                        add = it == posneg->second.end() || !it->second; //FIXME: check for head?
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
                                add = it == posneg->second.end() || it->second; //FIXME: check for head?
                        }
		}
		
		if(add) truerules.insert(*r);
	}

	return truerules;
}

bool Helper::trueRule(	const set<Variable> &positives, 
			const set<Variable> &all, 
			Rule rule, 
			const SignMap &signs)
{
	bool add = false;
	SignMap::const_iterator posneg = signs.find(rule);
	set<int>::iterator pit = positives.begin(), ait = all.begin();
	for(map<int, bool>::const_iterator pnit = posneg->second.begin(); !add && pnit != posneg->second.end(); ++pnit)
	{
		while(pit != positives.end() && *pit < pnit->first) ++pit;
		if(pit != positives.end() && *pit == pnit->first) { if(!pnit->second) { add = true; continue; } else continue; }
		while(ait != all.end() && *ait < pnit->first) ++ait;
		if(ait != all.end() && *ait == pnit->first && pnit->second) { add = true; continue; }
	}

	return add;
}

bool Helper::trueRule(	const set<Variable> &base,
			const set<Variable> &positives,
			const set<Variable> &all,
			Rule rule,
			const SignMap &signs,
			const HeadMap &heads)
{
	vector<Variable> v(all.size());
	set<Variable> negatives;
	set<Rule> rules; rules.insert(rule);
	vector<Variable>::iterator end;

	end = set_difference(	all.begin(), all.end(), 
				positives.begin(), positives.end(), 
				v.begin());

	negatives.insert(v.begin(), end);

	return trueRules(base, positives, negatives, rules, signs, heads).size() > 0;
}

bool Helper::trueRule(	const set<Variable> &positives,
			const set<Variable> &all,
			Rule rule,
			Variable variable,
			const Order &order,
			const OrderTypes &ordertypes,
			const SignMap &signs,
			const HeadMap &heads)
{
	//TODO
	set<Rule> rules; rules.insert(rule);

	return trueRules(positives, all, rules, variable, order, ordertypes, signs, heads).size() > 0;
}

OrderCombinations Helper::combineOrder(	const Order &original,
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

OrderCombinations Helper::combineOrder(	const Order &original,
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
						

OrderCombinations Helper::combineOrder(	const Order &left,
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

