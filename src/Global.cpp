#include <set>
#include <vector>
#include <algorithm>
#include <iostream>

using namespace std;

bool containsAll(const set<int> &a, const set<int> &b)
{
	if(a.size() < b.size()) return false;

	vector<int> temp(b.size());
	vector<int>::iterator it;

	it = set_intersection(a.begin(), a.end(), b.begin(), b.end(), temp.begin());

	if(it - temp.begin() == (int)b.size()) return true; else return false;
}

void printIntSet(const set<int> &toprint)
{
	const char *prefix = "";

	for(set<int>::iterator it = toprint.begin(); it != toprint.end(); ++it)
	{
		cout << prefix << *it;
		prefix = ", ";
	}	
}
