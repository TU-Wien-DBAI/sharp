#ifdef HAVE_CONFIG_H
	#include <config.h>
#endif

#include "IterativeTreeSolver.hpp"

#include "NullTreeSolutionExtractor.hpp"
#include "NodeTableMap.hpp"

#include <sharp/Benchmark.hpp>

#include <stack>
#include <memory>
#include <cstddef>

namespace sharp
{
	using htd::vertex_t;
	using htd::IHypergraph;
	using htd::ITreeDecomposition;
	using htd::ITreeDecompositionAlgorithm;

	using std::unique_ptr;
	using std::stack;
	using std::pair;
	using std::make_pair;
	using std::size_t;
	using std::string;
	using std::to_string;

	IterativeTreeSolver::IterativeTreeSolver(
			const ITreeDecompositionAlgorithm &decomposer,
			unique_ptr<const ITreeAlgorithm> algorithm)
		: decomposer_(decomposer), algorithm_(std::move(algorithm)),
		  extractor_(new NullTreeSolutionExtractor())
	{
		manageAlgorithmMemory_ = true;
		manageExtractorMemory_ = true;
	}

	IterativeTreeSolver::IterativeTreeSolver(
			const ITreeDecompositionAlgorithm &decomposer,
			unique_ptr<const ITreeAlgorithm> algorithm,
			unique_ptr<const ITreeSolutionExtractor> extractor)
		: decomposer_(decomposer), algorithm_(std::move(algorithm)),
		  extractor_(std::move(extractor))
	{
		manageAlgorithmMemory_ = true;
		manageExtractorMemory_ = true;
	}

	IterativeTreeSolver::IterativeTreeSolver(
			const ITreeDecompositionAlgorithm &decomposer,
			const ITreeAlgorithm &algorithm)
		: decomposer_(decomposer), algorithm_(&algorithm),
		  extractor_(new NullTreeSolutionExtractor())
	{
		manageAlgorithmMemory_ = false;
		manageExtractorMemory_ = true;
	}

	IterativeTreeSolver::IterativeTreeSolver(
			const ITreeDecompositionAlgorithm &decomposer,
			const ITreeAlgorithm &algorithm,
			const ITreeSolutionExtractor &extractor)
		: decomposer_(decomposer), algorithm_(&algorithm),
		  extractor_(&extractor)
	{
		manageAlgorithmMemory_ = false;
		manageExtractorMemory_ = false;
	}

	IterativeTreeSolver::~IterativeTreeSolver()
	{
		if(!manageAlgorithmMemory_) algorithm_.release();
		if(!manageExtractorMemory_) extractor_.release();
	}

	ITreeDecomposition *IterativeTreeSolver::decompose(
			const IInstance &instance) const
	{
		unique_ptr<IHypergraph> hg(instance.toHypergraph());
		//TODO: include preprocessingOperations in decomposition call
		//		once libhtd supports this
		ITreeDecomposition * td = decomposer_.computeDecomposition(*hg);
		Benchmark::registerTimestamp("tree decomposition time");
		return td;
	}

	ISolution *IterativeTreeSolver::solve(
			const IInstance &instance,
			const ITreeDecomposition &td) const
	{
		unique_ptr<INodeTableMap> tables(this->evaluate(td, instance));
		Benchmark::registerTimestamp("solving time");

		vertex_t root = td.root();
		ISolution *sol = nullptr;

		if(tables)
			sol = extractor_->extractSolution(root, td, *tables, instance);
		else
			sol = extractor_->emptySolution(instance);

		Benchmark::registerTimestamp("solution extraction time");

		return sol;
	}

	ISolution *IterativeTreeSolver::solve(const IInstance &instance) const
	{
		unique_ptr<ITreeDecomposition> td(this->decompose(instance));
		return this->solve(instance, *td);
	}

	unique_ptr<INodeTableMap> IterativeTreeSolver::evaluate(
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
			currentTable = algorithm_->evaluateNode(
											current,
											td,
											*tables,
											instance);

			if(currentTable) insertIntoMap(current, td, currentTable, *tables);
			else
			{
				tables.reset();
				break;
			}
		}

		return tables;
	}

	unique_ptr<INodeTableMap> IterativeTreeSolver::initializeMap(
			size_t decompositionNodeCount) const
	{
		return unique_ptr<INodeTableMap>(
				new NodeTableMap(decompositionNodeCount));
	}

	void IterativeTreeSolver::insertIntoMap(
			vertex_t node,
			const ITreeDecomposition &td,
			ITable *table,
			INodeTableMap &tables) const
	{
		IMutableNodeTableMap &map =
			dynamic_cast<IMutableNodeTableMap &>(tables);

		map.insert(node, table);
		
		if(!algorithm_->needAllTables())
		{
			size_t childCount = td.childCount(node);
			for(size_t childIndex = 0; childIndex < childCount; ++childIndex)
				map.erase(td.child(node, childIndex));
		}
	}

} // namespace sharp
