#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <set>
#include <map>
#include <vector>

extern bool containsAll(const set<int> &a, const set<int> &b);
extern void printIntSet(const set<int> &toprint);

typedef map<int, map<int, bool> > SignMap;
typedef pair<vector<set<int> >, vector<set<int> > > Partition;
typedef pair<set<int>, set<int> > Atom;
typedef set<int> Solution;
typedef set<Solution> SolutionSet;

#endif /*GLOBAL_H_*/
