#ifndef SHARP_HTDSOLUTIONTREEBUILDER_H_
#define SHARP_HTDSOLUTIONTREEBUILDER_H_

#include "HtdSolutionTree.hpp"

#include <libsharp/IHtdAlgorithm.hpp>
#include <libsharp/IInstance.hpp>

#include <vector>
#include <unordered_map>

namespace sharp
{
	typedef std::unordered_map<ITuple *, HtdSolutionTree *> tuple_solution_map;
	typedef std::unordered_map<vertex_t, tuple_solution_map> node_map;

	class HtdSolutionTreeBuilder
	{
	private:
		static void addToTSMap(ITuple *t, HtdSolutionTree *soltree, tuple_solution_map &tsmap);

	public:
		HtdSolutionTreeBuilder(const IHtdAlgorithm &algorithm, const IInstance &instance, const ITreeDecomposition &decomposition);
		~HtdSolutionTreeBuilder();

		std::vector<HtdSolutionTree *> buildTrees() const;
		HtdSolutionTree &buildNextTree() const;

	private:
		void buildTreesForNode(vertex_t node, node_map &nmap) const;
		void processLeafTuples(vertex_t node, ITupleIterator *iter, tuple_solution_map &tsmap) const;
		void processTransformTuples(vertex_t node, HtdSolutionTree &base, ITupleIterator *iter, tuple_solution_map &tsmap) const;
		void processJoinTuples(vertex_t node, HtdSolutionTree &base1, HtdSolutionTree &base2, ITupleIterator *iter, tuple_solution_map &tsmap) const;


		// externally visible state
		const IHtdAlgorithm &m_algorithm;
		const IInstance &m_instance;
		const ITreeDecomposition &m_decomposition;

	}; // struct HtdSolutionTreeBuilder

} // namespace sharp

namespace std
{
	template<>
	struct hash<sharp::ITuple *>
	{
	public:
		size_t operator()(sharp::ITuple * const &t) const { return t->hash(); }
	}; // class hash<sharp::ITuple *>

} // namespace std

#endif // SHARP_HTDSOLUTIONTREEBUILDER_H_
