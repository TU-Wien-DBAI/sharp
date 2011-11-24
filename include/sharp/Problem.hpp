#ifndef PROBLEM_H_
#define PROBLEM_H_

#include <sharp/Global.hpp>

namespace sharp
{
	class Hypergraph;
	class ExtendedHypertree;
	class AbstractHTDAlgorithm;
	class AbstractHypertreeDecompositionAlgorithm;
	class Solution;
	class Instantiator;

	class Problem
	{
	public:
		Problem(AbstractHypertreeDecompositionAlgorithm *htdalgorithm = NULL, bool collectStatistics = false);
		Problem(bool collectStatistics);
		virtual ~Problem();
		
	public:
		// gets the solution for the problem, that is:
		// - the input is parsed (parse method is called)
		// - preprocessing is done (preprocess method is called)
		// - the hypergraph representation is generated (buildHypergraphRepresentation)
		// - a tree decomposition is generated
		// - the actual algorithm is run (evaluate method of the Algorithm class)
		// - the solution for the problem is returned
		virtual Solution *calculateSolution(AbstractHTDAlgorithm *algorithm);
		Solution *calculateSolutionFromDecomposition(AbstractHTDAlgorithm *algorithm, ExtendedHypertree *hypertree);
	
		// calculates a tree decomposition from the input, that is:
		// - input is parsed (parse method is called)
		// - preprocessing is done (preprocess method is called)
		// - the hypergraph representation is generated (buildHypergraphRepresentation)
		// - a tree decomposition is generated
		// - preprocessing of tree decomposition (prepareHypertreeDecomposition method is called)
		ExtendedHypertree *calculateHypertreeDecomposition();

		// gets the internal ID of a vertex by its name
		Vertex getVertexId(std::string vertexName);
	
		// gets the number of vertices currently stored
		unsigned int getVertexCount();
	
		// gets the name of a vertex by its internal ID
		std::string getVertexName(Vertex vertexId);
	
		// prints the name mappings (for debug or verbose output)
		void printVertexNames(std::ostream &out);
	
	protected: 
		// called by calculateSolution if the problem has not yet been read
		virtual void parse() = 0;
	
		// called by the calculateSolution method, after preprocessing
		virtual Hypergraph *buildHypergraphRepresentation() = 0;
	
		// called by the calculateSolution method, before hypergraph is built 
		virtual void preprocess();

		// called by calculateHypertreeDecomposition, after tree decomposition is built
		virtual ExtendedHypertree *prepareHypertreeDecomposition(ExtendedHypertree *root);
	
		// helper methods, creates graph from vertex- and edge-sets
		static Hypergraph *createGraphFromSets(VertexSet vertices, EdgeSet edges);
		static Hypergraph *createGraphFromDisjointSets(VertexSet v1, VertexSet v2, EdgeSet edges);
		static Hypergraph *createHypergraphFromSets(VertexSet vertices, HyperedgeSet hyperedges);
	
		// stores a vertex name, assigns a new internal ID if it doesn't exist
		Vertex storeVertexName(std::string name);
	
		// creates a new auxiliary vertex and returns its interal ID
		Vertex createAuxiliaryVertex();

		// clears the store of internal vertex IDs
		void clearVertexStore();
	
	private:
		// private: common initialization method for the constructors
		void initialize(AbstractHypertreeDecompositionAlgorithm *htdalg, bool collectStatistics);

		// private: maps the internal IDs of vertices to names
		NameMap vertexNames;
		
		// private: maps the names of vertices to internal IDs
		ReverseNameMap reverseVertexNames;

		// private: the algorithm used to obtain the hypertree decomposition
		AbstractHypertreeDecompositionAlgorithm *htdalgorithm;
	
		// private: stores information about whether the input has already been parsed
		bool parsed;

		// private: capture statistics about internal data (timing, htd-properties, etc.)
		bool statistics;
	};

} // namespace sharp

#endif //PROBLEM_H_
