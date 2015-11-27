#ifdef HAVE_CONFIG_H
	#include <config.h>
#endif

#include "IterativeTreeSolverBase.hpp"

#include <stack>
#include <memory>
#include <cstddef>

namespace sharp
{
	//FIXME: to uppercase (constant), remove when htd library is updated
	using htd::unknown_vertex;
	#define UNKNOWN_VERTEX unknown_vertex

	using htd::ITreeDecomposition;
	using htd::vertex_t;

	using std::unique_ptr;
	using std::stack;
	using std::pair;
	using std::make_pair;
	using std::size_t;
	
	IterativeTreeSolverBase::IterativeTreeSolverBase() { }

	IterativeTreeSolverBase::~IterativeTreeSolverBase() { }

	ISolution *IterativeTreeSolverBase::solve(const IInstance &instance) const
	{
		unique_ptr<ITreeDecomposition> td(this->decompose(instance));
		unique_ptr<INodeTableMap> tables(this->evaluate(*td, instance));
		vertex_t root = td->root();

		if(tables)
			return this->extractSolution(root, *td, *tables, instance);
		else
			return this->emptySolution(instance);
	}

	unique_ptr<INodeTableMap> IterativeTreeSolverBase::evaluate(
			const ITreeDecomposition &td,
			const IInstance &instance) const
	{
		vertex_t current = td.root();
		ITable *currentTable = nullptr;
		stack<pair<vertex_t, size_t> > parents;
		unique_ptr<INodeTableMap> tables = 
			this->initializeMap(td.vertexCount());

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

			currentTable = this->evaluateNode(current, td, *tables, instance);

			if(currentTable)
			{
				insertIntoMap(current, td, currentTable, *tables);
			}
			else
			{
				tables.reset();
				break;
			}
		}

		return tables;
	}

} // namespace sharp
