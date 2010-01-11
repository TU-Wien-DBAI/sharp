#include <set>
#include <list>
#include <vector>
#include <algorithm>
#include <iostream>

#include "Global.h"

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

	for(set<int>::const_iterator it = toprint.begin(); it != toprint.end(); ++it)
	{
		cout << prefix << *it;
		prefix = ", ";
	}	
}

void printIntList(const list<int> &toprint)
{
	const char *prefix = "";
	
	for(list<int>::const_iterator it = toprint.begin(); it != toprint.end(); ++it)
	{
		cout << prefix << *it;
		prefix = ", ";
	}
}

void printBoolList(const list<bool> &toprint)
{
	const char *prefix = "";
	
	for(list<bool>::const_iterator it = toprint.begin(); it != toprint.end(); ++it)
	{
		cout << prefix << (*it ? "t" : "f");
		prefix = ", ";
	}
}

Timer::Timer() 
{
	this->start();
}

Timer::~Timer() { }

void Timer::start()
{
	getrusage(RUSAGE_SELF, &this->beginning);
}

pair<double, double> Timer::stop()
{
	double cpu, sys;
	
	getrusage(RUSAGE_SELF, &this->end);

	cpu = double(this->end.ru_utime.tv_sec - this->beginning.ru_utime.tv_sec) 
		+ double(this->end.ru_utime.tv_usec - this->beginning.ru_utime.tv_usec) / 1000000.0;
	sys = double(this->end.ru_stime.tv_sec - this->beginning.ru_stime.tv_sec) 
		+ double(this->end.ru_stime.tv_usec - this->beginning.ru_stime.tv_usec) / 1000000.0;

	return pair<double, double>(cpu, sys);
}
