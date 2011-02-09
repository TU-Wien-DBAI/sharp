#ifndef GLOBAL_H_
#define GLOBAL_H_

#include <ostream>
#include <set>
#include <map>
#include <vector>
#include <list>
#include <string>

#include <sharp/support.hpp>

namespace sharp
{

	bool containsAll(const std::set<unsigned int> &a, const std::set<unsigned int> &b);
	
	typedef unsigned int Vertex;
	#define VERTEXNOTFOUND (0)
	
	typedef std::set<Vertex> VertexSet;
	typedef std::set<std::pair<Vertex, Vertex> > EdgeSet;
	
	typedef std::vector<std::string> NameMap;
	typedef std::map<std::string, Vertex> ReverseNameMap;
	
	
	//old---------
	
	void printIntSet(const VertexSet &toprint);
	void printIntList(const std::list<Vertex> &toprint);
	void printIntVector(const VertexSet &toprint);
	void printBoolVector(const std::vector<bool> &toprint);
	void printBoolList(const std::list<bool> &toprint);
	
	/*
	typedef Vertex Variable;
	typedef Vertex Rule;
	typedef std::pair<std::set<Variable>, std::set<Rule> > Atom;
	
	typedef std::map<Rule, std::map<Variable, bool> > SignMap;
	typedef std::vector<std::string> NameMap;
	typedef std::vector<std::set<Variable> > HeadMap;
	typedef std::list<Vertex> Order;
	typedef std::list<bool> OrderTypes;
	typedef std::list<std::pair<Order, OrderTypes> > OrderCombinations;
	
	typedef std::pair<std::vector<std::set<Variable> >, std::vector<std::set<Variable> > > Partition;
	*/

} // namespace sharp

#endif /*GLOBAL_H_*/
