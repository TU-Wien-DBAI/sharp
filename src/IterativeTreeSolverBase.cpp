#ifdef HAVE_CONFIG_H
	#include <config.h>
#endif

#include "IterativeTreeSolverBase.hpp"

#include <stack>
#include <memory>
#include <cstddef>

namespace sharp
{
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
		bool finishedBranch = false;
		vertex_t current = td.root();
		ITable *currentTable = nullptr;
		stack<pair<vertex_t, size_t> > parents;
		unique_ptr<INodeTableMap> tables = 
			this->initializeMap(td.vertexCount());

		while(!parents.empty() || !finishedBranch)
		{
			if(!finishedBranch)
			{
				if(td.childCount(current) != 0)
				{
					parents.push(make_pair(current, 1));
					current = td.child(current, 0);
					continue;
				}

				parents.push(make_pair(current, 0));
				finishedBranch = true;
			}

			pair<vertex_t, size_t> top = parents.top();
			parents.pop();
			if(td.childCount(top.first) > top.second)
			{
				current = td.child(top.first, top.second);
				parents.push(make_pair(top.first, top.second + 1));
				finishedBranch = false;
				continue;
			}
			current = top.first;
			currentTable = this->evaluateNode(current, td, *tables, instance);

			if(currentTable) insertIntoMap(current, td, currentTable, *tables);
			else
			{
				tables.reset();
				break;
			}
		}

		return tables;
	}

} // namespace sharp
