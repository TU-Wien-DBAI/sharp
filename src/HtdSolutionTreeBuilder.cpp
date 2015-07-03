#include "HtdSolutionTreeBuilder.hpp"

#include <stack>
#include <utility>

namespace sharp
{
	using htd::UNKNOWN_VERTEX;

	HtdSolutionTreeBuilder::HtdSolutionTreeBuilder(const IHtdAlgorithm &algorithm, const IInstance &instance, const ITreeDecomposition &decomposition)
		: m_algorithm(algorithm), m_instance(instance), m_decomposition(decomposition)
	{ }

	HtdSolutionTreeBuilder::~HtdSolutionTreeBuilder() { }

	std::vector<HtdSolutionTree *> HtdSolutionTreeBuilder::buildTrees() const
	{
		vertex_t current = this->m_decomposition.root();	
		std::stack<std::pair<vertex_t, int> > parents;
		node_map nmap;
		
		while(!parents.empty() || current != UNKNOWN_VERTEX)
		{
			if(current != UNKNOWN_VERTEX)
			{
				parents.push(std::make_pair(current, 1));
				current = this->m_decomposition.child(current, 0);
				continue;
			}

			std::pair<vertex_t, int> top = parents.top();
			current = this->m_decomposition.child(top.first, top.second);
			parents.pop();

			if(current != UNKNOWN_VERTEX)
			{
				parents.push(std::make_pair(top.first, top.second + 1));
				continue;
			}
			
			this->buildTreesForNode(current, nmap);
		}

		tuple_solution_map &rootmap = nmap[this->m_decomposition.root()];

		std::vector<HtdSolutionTree *> result;
		result.reserve(rootmap.size());
		for(auto kv : rootmap)
		{
			result.push_back(kv.second);
			delete kv.first;
		}

		return result;
	}

	void HtdSolutionTreeBuilder::buildTreesForNode(vertex_t node, node_map &nmap) const
	{
		vertex_t firstchild = this->m_decomposition.child(node, 0);

		if(firstchild == UNKNOWN_VERTEX) // we are processing a leaf node
		{
			this->processLeafTuples(node, this->m_algorithm.computeLeaf(node, this->m_decomposition, this->m_instance), nmap[node]);
			return;
		}

		vertex_t nextchild = this->m_decomposition.child(node, 1);

		if(nextchild == UNKNOWN_VERTEX) // we are processing a transformation node
		{
			for(const auto &tspair : nmap[firstchild])
			{
				this->processTransformTuples(node, *tspair.second, this->m_algorithm.computeTransform(*tspair.first, node, firstchild, this->m_decomposition, this->m_instance), nmap[node]);

				if(tspair.second->hasParent())
					delete tspair.second;
				delete tspair.first;
			}
			nmap.erase(firstchild);
			return;
		}

		// we are processing a join node
		int childindex = 1;
		vertex_t currentchild = firstchild;
		tuple_solution_map old, current;
		tuple_solution_map &tsmap1 = nmap[firstchild], &tsmap2 = nmap[nextchild];
		do
		{						
			tsmap2 = nmap[nextchild];

			for(const auto &tspair1 : tsmap1)
			{
				for(const auto &tspair2 : tsmap2)
					this->processJoinTuples(node, *tspair1.second, *tspair2.second, this->m_algorithm.computeJoin(*tspair1.first, *tspair2.first, node, currentchild, nextchild, this->m_decomposition, this->m_instance), current);

				if(tspair1.second->hasParent())
					delete tspair1.second;
				delete tspair1.first;
			}
			
			// TODO: find a better way to delete tsmap2 contents (e.g. mark/count in the loop above?)
			// SOLUTION: have first for-loop go via iterators, not processing the last element
			// SOLUTION: then process the last element with separate code to delete also tspair2 stuff
			for(const auto &tspair2 : tsmap2)
			{
				if(tspair2.second->hasParent())
					delete tspair2.second;
				delete tspair2.first;
			}

			old = std::move(current);
			current.clear();
			tsmap1 = old;
			currentchild = node;
			nmap.erase(nextchild);
			nextchild = this->m_decomposition.child(node, ++childindex);
		}
		while(nextchild != UNKNOWN_VERTEX);
		nmap.erase(firstchild);

		nmap[node] = std::move(current);
	}	

	void HtdSolutionTreeBuilder::addToTSMap(ITuple *t, HtdSolutionTree *soltree, tuple_solution_map &tsmap)
	{
		auto result = tsmap.insert(std::make_pair(t, soltree));
		if(!result.second) result.first->second = new HtdSolutionTree(result.first->second, soltree);
	}


	void HtdSolutionTreeBuilder::processJoinTuples(vertex_t node, HtdSolutionTree &base1, HtdSolutionTree &base2, ITupleIterator *iter, tuple_solution_map &tsmap) const
	{
		ITuple *t = nullptr;

		while((t = iter->next()) != nullptr)
		{
			HtdSolutionTree *soltree = new HtdSolutionTree(&base1, &base2, t->extractTupleSolution(node, this->m_decomposition, this->m_instance));
			HtdSolutionTreeBuilder::addToTSMap(t, soltree, tsmap);
		}

		delete iter;
	}

	void HtdSolutionTreeBuilder::processTransformTuples(vertex_t node, HtdSolutionTree &base, ITupleIterator *iter, tuple_solution_map &tsmap) const
	{
		ITuple *t = nullptr;

		while((t = iter->next()) != nullptr)
		{
			HtdSolutionTree *soltree = new HtdSolutionTree(&base, t->extractTupleSolution(node, this->m_decomposition, this->m_instance));
			HtdSolutionTreeBuilder::addToTSMap(t, soltree, tsmap);
		}

		delete iter;
	}

	void HtdSolutionTreeBuilder::processLeafTuples(vertex_t node, ITupleIterator *iter, tuple_solution_map &tsmap) const
	{
		ITuple *t = nullptr;

		while((t = iter->next()) != nullptr)
		{
			HtdSolutionTree *soltree = new HtdSolutionTree(t->extractTupleSolution(node, this->m_decomposition, this->m_instance));
			HtdSolutionTreeBuilder::addToTSMap(t, soltree, tsmap);
		}

		delete iter;
	}

	HtdSolutionTree &HtdSolutionTreeBuilder::buildNextTree() const
	{
		// TODO
		return *new HtdSolutionTree(nullptr);
	}
}
