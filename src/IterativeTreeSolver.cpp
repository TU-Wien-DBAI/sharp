#ifdef HAVE_CONFIG_H
	#include <config.h>
#endif

#include "IterativeTreeSolver.hpp"

#include "TableMap.hpp"

#include <stack>

#include <cstdlib>

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
			const ITreeEvaluator &evaluator)
		: impl(new Impl(decomposer, evaluator))
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
			const ITreeEvaluator &evaluator)
		: decomposer_(decomposer), evaluator_(evaluator)
	{ }

	IterativeTreeSolver::Impl::~Impl()
	{ }

	ISolution *IterativeTreeSolver::Impl::solve(
			const IInstance &instance) const
	{
		unique_ptr<ITreeDecomposition> td(decompose(instance));
		unique_ptr<ITableMap> tableMap(evaluate(*td, instance));
		vertex_t root = td->root();

		return evaluator_.calculateSolution(root,
				*td, *tableMap, instance);
	}

	unique_ptr<ITreeDecomposition> IterativeTreeSolver::Impl::decompose(
			const IInstance &instance) const
	{
		unique_ptr<IHypergraph> hg(instance.toHypergraph());
		return unique_ptr<ITreeDecomposition>(
				decomposer_.computeDecomposition(*hg));
	}

	unique_ptr<ITableMap> IterativeTreeSolver::Impl::initializeTableMap(
			size_t size) const
	{
		return unique_ptr<ITableMap>(new TableMap(size));
	}

	void IterativeTreeSolver::Impl::updateTableMap(
			vertex_t node,
			ITable *table,
			ITableMap &tableMap) const
	{
			tableMap.addTable(node, table);
	}


	unique_ptr<ITableMap> IterativeTreeSolver::Impl::evaluate(
			const ITreeDecomposition &td,
			const IInstance &instance) const
	{
		vertex_t current = td.root();
		stack<pair<vertex_t, size_t> > parents;
		unique_ptr<ITableMap> tableMap(initializeTableMap(td.vertexCount()));
		ITable *currentTable = nullptr;

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

			currentTable = evaluator_.evaluateNode(current,
					td, *tableMap, instance);

			updateTableMap(current, currentTable, *tableMap);
		}

		return tableMap;
	}

} // namespace sharp
