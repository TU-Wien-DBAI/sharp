#ifndef PROBLEM_H_
#define PROBLEM_H_

#include <string>

#include <sharp/Global.hpp>
#include <sharp/Hypergraph.hpp>
#include <sharp/ExtendedHypertree.hpp>
#include <sharp/AbstractAlgorithm.hpp>

namespace sharp
{

	enum DecompositionOptions
	{
		None = 0,
		BipartiteGraph = 1
	};
	
	class Problem
	{
	public:
		Problem(AbstractAlgorithm *algorithm, bool printBenchmarkInformation = false);
		virtual ~Problem();
		
	public:
		// gets the solution for the problem, that is:
		// - the input is parsed (parse method is called))
		// - preprocessing is done (preprocess method is called)
		// - the hypergraph representation is generated (buildHypergraphRepresentation)
		// - a tree decomposition is generated
		// - the actual algorithm is run (evaluate method of the Algorithm class)
		// - the solution for the problem is returned
		Solution *calculateSolution(Instantiator *instantiator);
	
		// gets the treewidth for the problem, that is:
		// - the input is parsed (parse method is called)
		// - preprocessing is done (preprocess method is called)
		// - the hypergraph representation is generated (buildHypergraphRepresentation)
		// - a tree decomposition is generated
		// - the tree width is returned
		int calculateTreeWidth();
	
		// set the decomposition options for the decomposition routine
		void setDecompositionOptions(DecompositionOptions options = None, void *parameter = NULL);
	
		// gets the internal ID of a vertex by its name
		Vertex getVertexId(std::string vertexName);
	
		// gets the number of vertices currently stored
		unsigned int getVertexCount();
	
		// gets the name of a vertex by its internal ID
		std::string getVertexName(Vertex vertexId);
	
		// prints the name mappings (for debug or verbose output)
		void printVertexNames(ostream &out);
	
	protected: 
		// called by calculateSolution if the problem has not yet been read
		virtual void parse() = 0;
	
		// called by the calculateSolution method, before hypergraph is built 
		virtual void preprocess() = 0;
	
		// called by the calculateSolution method, after preprocessing
		virtual Hypergraph *buildHypergraphRepresentation() = 0;
	
		// helper methods, creates graph from vertex- and edge-sets
		static Hypergraph *createGraphFromSets(VertexSet vertices, EdgeSet edges);
		static Hypergraph *createGraphFromDisjointSets(VertexSet v1, VertexSet v2, EdgeSet edges);
	
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
	
		// private: stores if the input has already been parsed
		bool parsed;
	
		// private: print benchmark timing information for each step
		bool benchmark;
	
		// private: the decomposition options
		DecompositionOptions decompositionOptions;
	
		// private: the decomposition parameter (if any)
		void *decompositionParameter;
	};

} // namespace sharp

#endif //PROBLEM_H_
