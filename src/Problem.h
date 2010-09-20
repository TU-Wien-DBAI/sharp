#ifndef PROBLEM_H_
#define PROBLEM_H_

#include <string>

#include "Global.h"
#include "htree/Hypergraph.h"
#include "sharp/AbstractAlgorithm.h"

class Problem
{
public:
	Problem(AbstractAlgorithm *algorithm);
	virtual ~Problem();
	
public:
	// gets the solution for the problem, that is:
	// - the input is parsed (parse method is called))
	// - preprocessing is done (preprocess method is called)
	// - the hypergraph representation is generated (byildHypergraphRepresentation)
	// - a tree decomposition is generated
	// - the actual algorithm is run (evaluate method of the Algorithm class)
	// - the solution for the problem is returned
	Solution *calculateSolution(Instantiator *instantiator);

	// gets the internal ID of a vertex by its name
	Vertex getVertexId(std::string vertexName);

	// gets the name of a vertex by its internal ID
	std::string getVertexName(Vertex vertexId);

protected: 
	// called by calculateSolution if the problem has not yet been read
	virtual void parse() = 0;

	// called by the calculateSolution method, before hypergraph is built 
	virtual void preprocess() = 0;

	// called by the calculateSolution method, after preprocessing
	virtual Hypergraph *buildHypergraphRepresentation() = 0;

	// helper method, creates hypergraph from vertex- and edge-sets
	static Hypergraph *createHypergraphFromSets(VertexSet vertices, EdgeSet edges);

	// stores a vertex name, assigns a new internal ID if it doesn't exist
	Vertex storeVertexName(std::string name);

	// creates a new auxiliary vertex and returns its interal ID
	Vertex createAuxiliaryVertex();

private:
	// private: maps the internal IDs of vertices to names
	NameMap vertexNames;
	
	// private: maps the names of vertices to internal IDs
	ReverseNameMap reverseVertexNames;

	// private: the algorithm that is used to solve the problem
	AbstractAlgorithm *algorithm;
};

#endif //PROBLEM_H_
