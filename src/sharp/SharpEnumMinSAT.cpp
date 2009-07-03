#include <cmath>
#include <algorithm>

#include <iostream>

using namespace std;

#include "../support/support.h"
#include "SharpEnumMinSAT.h"

typedef SharpValue SV;

static void printSharpValues(set<SV> &svals)
{
        cout << "SVALS: " << svals.size() << endl;
        for(set<SV>::iterator it = svals.begin(); it != svals.end(); ++it)
        {
                cout << "var: "; printIntSet(it->assignmentVariables);
                cout << ", cla: "; printIntSet(it->assignmentClauses);
		cout << ", gua: "; 
		for(set<Atom>::iterator i = it->guards.begin(); i != it->guards.end(); ++i)
		{ cout << "[>"; printIntSet(i->first); cout << "<|>"; printIntSet(i->second); cout << "<], "; }
                cout << ", val: " << it->value;
		cout << ", sol: ";
		for(set<set<int> >::iterator i = it->solutions.begin(); i != it->solutions.end(); ++i)
		{ cout << "["; printIntSet(*i); cout << "], "; }
		cout << endl;
        }
        cout << "---" << endl;
}


bool SharpValue::operator<(const SharpValue &other) const
{
	less<set<int> > ls; equal_to<set<int> > es;
	less<set<Atom> > lsa;

	return ls(this->assignmentVariables, other.assignmentVariables)
		|| (es(this->assignmentVariables, other.assignmentVariables) 
			&& ls(this->assignmentClauses, other.assignmentClauses))
		|| (es(this->assignmentVariables, other.assignmentVariables) 
			&& es(this->assignmentClauses, other.assignmentClauses) 
			&& lsa(this->guards, other.guards));
}

SharpEnumMinSAT::SharpEnumMinSAT(ExtendedHypertree *root, signmap &signs) : signs(signs)
{
        this->root = root;
}

SharpEnumMinSAT::~SharpEnumMinSAT()
{
}

Partition SharpEnumMinSAT::partition(const set<int> &variables)
{
	typedef set<int> set_t;
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

set<int> SharpEnumMinSAT::trueClauses(const set<int> &positives, const set<int> &negatives, const set<int> &clauses) const
{
	//FIXME very slow, utilize the fact that sets are sorted
        set<int> trueclauses;

        for(set<int>::const_iterator cl = clauses.begin(); cl != clauses.end(); ++cl)
        {
                bool add = false;
                for(set<int>::const_iterator var = positives.begin(); !add && var != positives.end(); ++var)
                {
                        map<int, bool> &posneg = this->signs[*cl];
                        map<int, bool>::iterator it = posneg.find(*var);
                        add = it != posneg.end() && !it->second;
                }
                for(set<int>::const_iterator var = negatives.begin(); !add && var != negatives.end(); ++var)
                {
                        map<int, bool> &posneg = this->signs[*cl];
                        map<int, bool>::iterator it = posneg.find(*var);
                        add = it != posneg.end() && it->second;
                }
                if(add) trueclauses.insert(*cl);
        }

        return trueclauses;
}

bool SharpEnumMinSAT::isTrue(const set<int> &positives, const set<int> &all, int clause) const
{
	bool add = false;
	map<int, bool> &posneg = this->signs[clause];
	set<int>::iterator pit = positives.begin(), ait = all.begin();
	for(map<int, bool>::iterator pnit = posneg.begin(); !add && pnit != posneg.end(); ++pnit)
	{
		while(pit != positives.end() && *pit < pnit->first) ++pit;
		if(pit != positives.end() && *pit == pnit->first) { if(!pnit->second) { add = true; continue; } else continue; }
		while(ait != all.end() && *ait < pnit->first) ++ait;
		if(ait != all.end() && *ait == pnit->first && pnit->second) { add = true; continue; }
	}

	return add;
}


pair<mpz_class, set<set<int> > > SharpEnumMinSAT::evaluate() const
{
	//TODO

	cout << "start" << endl;
	set<SV> &result = eval(this->root);
	list<SV> solutions; equal_to<set<int> > eq;
	set<int> clauses = this->root->getClauses();
	mpz_class sum = 0; set<set<int> > models;

	for(set<SV>::iterator it = result.begin(); it != result.end(); ++it)
	{
		if(!eq(clauses, it->assignmentClauses)) continue;

		bool filter = false;
		for(set<Atom>::iterator git = it->guards.begin(); !filter && git != it->guards.end(); ++git)
		{
			if(git->second.size() >= clauses.size()) { filter = true; continue; }
			
			set<int>::iterator cit = clauses.begin();
			for(set<int>::iterator cgit = git->second.begin(); !filter && cgit != git->second.end(); ++cgit)
			{
				while(cit != clauses.end() && *cit < *cgit) ++cit;
				if(cit == clauses.end() || *cit != *cgit) filter = true;
			}
		}

		if(!filter) solutions.push_back(*it);
	}

	for(list<SV>::iterator it = solutions.begin(); it != solutions.end(); ++it)
	{
		sum += it->value;
		models.insert(it->solutions.begin(), it->solutions.end());
	}

	return pair<mpz_class, set<set<int> > >(sum, models);
}

set<SV> &SharpEnumMinSAT::eval(ExtendedHypertree *node) const
{
	switch(node->getType())
	{
	case ExtendedHypertree::LEAF: return this->evalLeaf(node);
        case ExtendedHypertree::VARREM: return this->evalVariableRemoval(node);
        case ExtendedHypertree::VARINTR: return this->evalVariableIntroduction(node);
        case ExtendedHypertree::BRANCH: return this->evalBranch(node);
        case ExtendedHypertree::CLREM: return this->evalClauseRemoval(node);
        case ExtendedHypertree::CLINTR: return this->evalClauseIntroduction(node);
	}

	CNULL(NULL); return *((set<SV> *)NULL);
}

set<SV> &SharpEnumMinSAT::evalLeaf(ExtendedHypertree *node) const
{
	//TODO

	cout << "LEAFA" << endl;

        set<SV> &svs = *new set<SV>();

	Partition apart = partition(node->getVariables());

        for(unsigned int i = 0; i < apart.first.size(); ++i)
	{
		SV sv;

		Partition gpart = partition(apart.first[i]);

		for(unsigned int j = 0; j < gpart.first.size(); ++j)
			if(gpart.first[j].size() != apart.first[i].size())
			{
				gpart.second[j].insert(apart.second[i].begin(), apart.second[i].end());
				sv.guards.insert(Atom(
					gpart.first[j],
					trueClauses(gpart.first[j], gpart.second[j], node->getClauses())));
			}

		sv.assignmentVariables = apart.first[i];
		sv.assignmentClauses = trueClauses(sv.assignmentVariables, apart.second[i], node->getClauses());

		sv.value = 1;
		sv.solutions.insert(set<int>(apart.first[i]));

		svs.insert(sv);
	}

	cout << "LEAFE" << endl;
	printSharpValues(svs);

        return svs;	
}

set<SV> &SharpEnumMinSAT::evalBranch(ExtendedHypertree *node) const
{
	//TODO


	set<SV> &left = eval(node->firstChild()), &right = eval(node->secondChild());
	set<SV> &svs = *new set<SV>();
	equal_to<set<int> > eq;
	cout << "BRA" << endl;

	for(set<SV>::iterator lit = left.begin(); lit != left.end(); ++lit)
		for(set<SV>::iterator rit = right.begin(); rit != right.end(); ++rit)
		{
			if(!eq(lit->assignmentVariables, rit->assignmentVariables)) continue;

			SV sv;

			for(set<Atom>::iterator glit = lit->guards.begin(); glit != lit->guards.end(); ++glit)
			{
				if(!eq(rit->assignmentVariables, glit->first)) continue;
				Atom g; g.first = rit->assignmentVariables; 
				g.second = rit->assignmentClauses;
				g.second.insert(glit->second.begin(), glit->second.end());
				sv.guards.insert(g);
			}

			lit->guards.insert(Atom(lit->assignmentVariables, lit->assignmentClauses));

			for(set<Atom>::iterator glit = lit->guards.begin(); glit != lit->guards.end(); ++glit)
				for(set<Atom>::iterator grit = rit->guards.begin(); grit != rit->guards.end(); ++grit)
				{
					if(!eq(glit->first, grit->first)) continue;
					Atom g; g.first = glit->first;
					g.second = glit->second;
					g.second.insert(grit->second.begin(), grit->second.end());
					sv.guards.insert(g);
				}

			sv.assignmentVariables = lit->assignmentVariables;
			sv.assignmentClauses = lit->assignmentClauses;
			sv.assignmentClauses.insert(rit->assignmentClauses.begin(), rit->assignmentClauses.end());
			
			sv.value = lit->value * rit->value;
			
			for(set<set<int> >::iterator i = lit->solutions.begin(); i != lit->solutions.end(); ++i)
				for(set<set<int> >::iterator j = rit->solutions.begin(); j != rit->solutions.end(); ++j)
				{
					set<int> ns = *i;
					ns.insert(j->begin(), j->end());
					sv.solutions.insert(ns);
				}

			pair<set<SV>::iterator, bool> result = svs.insert(sv);
			if(!result.second)
			{
				result.first->value += sv.value;
				result.first->solutions.insert(sv.solutions.begin(), sv.solutions.end());
			}
		}

	cout << "BRE" << endl;
	printSharpValues(svs);

	delete &left;
	delete &right;

	return svs;
}

set<SV> &SharpEnumMinSAT::evalVariableIntroduction(ExtendedHypertree *node) const
{
	//TODO


	set<SV> &base = eval(node->firstChild());
	set<SV> &svs = *new set<SV>();
	cout << "VIA" << endl;

	set<int> var; var.insert(node->getDifference());
	set<int> trueN = trueClauses(set<int>(), var, node->getClauses());
	set<int> trueP = trueClauses(var, set<int>(), node->getClauses());

	for(set<SV>::iterator it = base.begin(); it != base.end(); ++it)
	{
		SV svt, svf;
		
		svt.assignmentVariables = it->assignmentVariables;
		svt.assignmentVariables.insert(node->getDifference());
		svf.assignmentVariables = it->assignmentVariables;

		svt.assignmentClauses = it->assignmentClauses;
		svt.assignmentClauses.insert(trueP.begin(), trueP.end());
		svf.assignmentClauses = it->assignmentClauses;
		svf.assignmentClauses.insert(trueN.begin(), trueN.end());

		svt.value = svf.value = it->value;

		svf.solutions = it->solutions;
		for(set<set<int> >::iterator sit = it->solutions.begin(); sit != it->solutions.end(); ++sit)
		{ set<int> temp(*sit); temp.insert(node->getDifference()); svt.solutions.insert(temp); }

		for(set<Atom>::iterator git = it->guards.begin(); git != it->guards.end(); ++git)
		{
			Atom tempt(git->first, git->second), tempf(git->first, git->second);
			tempt.first.insert(node->getDifference());
			tempt.second.insert(trueP.begin(), trueP.end());
			tempf.second.insert(trueN.begin(), trueN.end());

			svt.guards.insert(tempt);
			svt.guards.insert(tempf);
			svf.guards.insert(tempf);
		}

		svt.guards.insert(Atom(svf.assignmentVariables, svf.assignmentClauses));
		
		pair<set<SV>::iterator, bool> result = svs.insert(svt);
		if(!result.second)
		{
			result.first->value += svt.value;
			result.first->solutions.insert(svt.solutions.begin(), svt.solutions.end());
		}

		result = svs.insert(svf);
		if(!result.second)
		{
			result.first->value += svf.value;
			result.first->solutions.insert(svf.solutions.begin(), svf.solutions.end());
		}
	}

	cout << "VIE" << endl;
	printSharpValues(svs);

	delete &base;
	return svs;
}

set<SV> &SharpEnumMinSAT::evalVariableRemoval(ExtendedHypertree *node) const
{
	//TODO


	set<SV> &base = eval(node->firstChild());
	set<SV> &svs = *new set<SV>();
	cout << "VRA" << endl;

	for(set<SV>::iterator it = base.begin(); it != base.end(); ++it)
	{
		SV sv;
		for(set<int>::iterator vit = it->assignmentVariables.begin(); vit != it->assignmentVariables.end(); ++vit)
			if(*vit != node->getDifference()) sv.assignmentVariables.insert(*vit);
		
		sv.assignmentClauses = it->assignmentClauses;
		sv.value = it->value;
		sv.solutions = it->solutions;

		for(set<Atom>::iterator git = it->guards.begin(); git != it->guards.end(); ++git)
		{
			Atom temp(set<int>(), git->second);
			for(set<int>::iterator vgit = git->first.begin(); vgit != git->first.end(); ++vgit)
				if(*vgit != node->getDifference()) temp.first.insert(*vgit);
			sv.guards.insert(temp);
		}

		pair<set<SV>::iterator, bool> result = svs.insert(sv);
		if(!result.second)
		{
			result.first->value += sv.value;
			result.first->solutions.insert(sv.solutions.begin(), sv.solutions.end());
		}
	}

	cout << "VRE" << endl;
	printSharpValues(svs);


	delete &base;
	return svs;
}

set<SV> &SharpEnumMinSAT::evalClauseIntroduction(ExtendedHypertree *node) const
{
	//TODO


	set<SV> &base = eval(node->firstChild());
	set<SV> &svs = *new set<SV>();
	cout << "CIA" << endl;

	for(set<SV>::iterator it = base.begin(); it != base.end(); ++it)
	{
		SV sv; sv.assignmentVariables = it->assignmentVariables;

		sv.assignmentClauses = it->assignmentClauses;
		if(isTrue(sv.assignmentVariables, node->getVariables(), node->getDifference())) sv.assignmentClauses.insert(node->getDifference());

		sv.value = it->value;
		sv.solutions = it->solutions;

		for(set<Atom>::iterator git = it->guards.begin(); git != it->guards.end(); ++git)
		{
			Atom temp(git->first, git->second);
			if(isTrue(temp.first, node->getVariables(), node->getDifference())) temp.second.insert(node->getDifference());
			sv.guards.insert(temp);
		}
	
		//FIXME implement a postproc function to avoid duplicate code (all of the eval functions!)
		pair<set<SV>::iterator, bool> result = svs.insert(sv);
		if(!result.second)
		{
			result.first->value += sv.value;
			result.first->solutions.insert(sv.solutions.begin(), sv.solutions.end());
		}
	}

	cout << "CIE" << endl;
	printSharpValues(svs);

	delete &base;
	return svs;	
}

set<SV> &SharpEnumMinSAT::evalClauseRemoval(ExtendedHypertree *node) const
{
	//TODO

	set<SV> &base = eval(node->firstChild());
	set<SV> &svs = *new set<SV>();
	cout << "CRA" << endl;

	for(set<SV>::iterator it = base.begin(); it != base.end(); ++it)
	{
		if(it->assignmentClauses.find(node->getDifference()) == it->assignmentClauses.end()) continue;
		
		SV sv; sv.assignmentVariables = it->assignmentVariables;
		
		for(set<int>::iterator cit = it->assignmentClauses.begin(); cit != it->assignmentClauses.end(); ++cit)
			if(*cit != node->getDifference()) sv.assignmentVariables.insert(*cit);

		sv.value = it->value;
		sv.solutions = it->solutions;

		for(set<Atom>::iterator git = it->guards.begin(); git != it->guards.end(); ++git)
		{
			if(git->second.find(node->getDifference()) == git->second.end()) continue;

			Atom temp(git->first, set<int>());
			for(set<int>::iterator cgit = git->second.begin(); cgit != git->second.end(); ++cgit)
				if(*cgit != node->getDifference()) temp.second.insert(*cgit);
			sv.guards.insert(temp);
		}

		pair<set<SV>::iterator, bool> result = svs.insert(sv);
		if(!result.second)
		{
			result.first->value += sv.value;
			result.first->solutions.insert(sv.solutions.begin(), sv.solutions.end());
		}
	}

	cout << "CRE" << endl;
	printSharpValues(svs);

	delete &base;
	return svs;
}

