#include <cmath>

#include <algorithm>

#include "Helper.h"

using namespace std;

Partition Helper::partition(const set<Variable> &variables)
{
	typedef set<Variable> set_t;
        typedef set_t::iterator iter_t;

	int size = int(pow(2, variables.size()));

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
                        SignMap::const_iterator posneg = signs.find(*r);
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
