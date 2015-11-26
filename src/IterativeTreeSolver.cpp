#ifdef HAVE_CONFIG_H
	#include <config.h>
#endif

#include "IterativeTreeSolver.hpp"

#include "NullTreeSolutionExtractor.hpp"
#include "NodeTableMap.hpp"

#include <stack>
#include <memory>
#include <cstddef>

namespace sharp
{
	//FIXME: to uppercase (constant), remove when htd library is updated
	using htd::unknown_vertex;
	#define UNKNOWN_VERTEX unknown_vertex

	using htd::vertex_t;
	using htd::IHypergraph;
	using htd::ITreeDecomposition;
	using htd::ITreeDecompositionAlgorithm;

	using std::unique_ptr;
	using std::stack;
	using std::pair;
	using std::make_pair;
	using std::size_t;
	
	IterativeTreeSolver::IterativeTreeSolver(
			const ITreeDecompositionAlgorithm &decomposer,
			const ITreeAlgorithm &algorithm)
		: impl(new Impl(decomposer, algorithm, NullTreeSolutionExtractor()))
	{ }

	IterativeTreeSolver::IterativeTreeSolver(
			const ITreeDecompositionAlgorithm &decomposer,
			const ITreeAlgorithm &algorithm,
			const ITreeSolutionExtractor &extractor)
		: impl(new Impl(decomposer, algorithm, extractor))
	{ }

	IterativeTreeSolver::~IterativeTreeSolver()
	{
		delete this->impl;
	}

	ISolution *IterativeTreeSolver::solve(const IInstance &instance) const
	{
		return this->impl->solve(instance);
	}

	IterativeTreeSolver::Impl::Impl(
			const ITreeDecompositionAlgorithm &decomposer,
			const ITreeAlgorithm &algorithm,
			const ITreeSolutionExtractor &extractor)
		: decomposer_(decomposer), algorithm_(algorithm), extractor_(extractor)
	{ }

	IterativeTreeSolver::Impl::~Impl()
	{ }

	ISolution *IterativeTreeSolver::Impl::solve(const IInstance &instance) const
	{
		unique_ptr<ITreeDecomposition> td(decompose(instance));
		unique_ptr<IMutableNodeTableMap> tableMap = evaluate(*td, instance);
		vertex_t root = td->root();

		if(tableMap)
			return extractor_.extractSolution(root, *td, *tableMap, instance);
		else
			return extractor_.emptySolution(instance);
	}

	unique_ptr<ITreeDecomposition> IterativeTreeSolver::Impl::decompose(
			const IInstance &instance) const
	{
		unique_ptr<IHypergraph> hg(instance.toHypergraph());
		return unique_ptr<ITreeDecomposition>(
				decomposer_.computeDecomposition(*hg));
	}

	unique_ptr<IMutableNodeTableMap> IterativeTreeSolver::Impl::initializeMap(
			size_t size) const
	{
		return unique_ptr<IMutableNodeTableMap>(new NodeTableMap(size));
	}

	void IterativeTreeSolver::Impl::insertIntoMap(
			vertex_t node,
			const ITreeDecomposition &td,
			ITable *table,
			IMutableNodeTableMap &tableMap) const
	{
		tableMap.insert(node, table);
		
		size_t childCount = td.childrenCount(node);
		for(size_t childIndex = 0; childIndex < childCount; ++childIndex)
			tableMap.erase(td.child(node, childIndex));
	}


	unique_ptr<IMutableNodeTableMap> IterativeTreeSolver::Impl::evaluate(
			const ITreeDecomposition &td,
			const IInstance &instance) const
	{
		vertex_t current = td.root();
		ITable *currentTable = nullptr;
		stack<pair<vertex_t, size_t> > parents;
		unique_ptr<IMutableNodeTableMap> tableMap(
				initializeMap(td.vertexCount()));

		while(!parents.empty() || current != UNKNOWN_VERTEX)
		{
			if(current != UNKNOWN_VERTEX)
			{
				if(td.childrenCount(current) != 0)
				{
					parents.push(make_pair(current, 1));
					current = td.child(current, 0);
				}
				else current = UNKNOWN_VERTEX;
				continue;
			}

			pair<vertex_t, size_t> top = parents.top();
			if(td.childrenCount(top.first) > top.second)
			{
				current = td.child(top.first, top.second);
				parents.pop();
				parents.push(make_pair(top.first, top.second + 1));
				continue;

			}
			else current = UNKNOWN_VERTEX;

			currentTable = algorithm_.evaluateNode(current,
					td, *tableMap, instance);

			if(currentTable)
			{
				insertIntoMap(current, td, currentTable, *tableMap);
			}
			else
			{
				tableMap.reset();
				break;
			}
		}

		return tableMap;
	}

} // namespace sharp
