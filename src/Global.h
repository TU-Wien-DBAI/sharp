#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <set>
#include <map>

extern bool containsAll(const set<int> &a, const set<int> &b);
extern void printIntSet(const set<int> &toprint);

typedef map<int, map<int, bool> > signmap;

#endif /*GLOBAL_H_*/
