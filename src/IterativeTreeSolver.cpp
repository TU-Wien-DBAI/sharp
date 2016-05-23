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
			const htd::ITreeDecompositionAlgorithm &decomposer,
			std::vector<std::unique_ptr<const ITreeAlgorithm> > &&algorithms,
			bool deleteAlgorithms)
		: IterativeTreeSolver(decomposer,
				std::move(algorithms),
				std::unique_ptr<const ITreeSolutionExtractor>(
					new NullTreeSolutionExtractor()),
				deleteAlgorithms,
				true)
	{ }

	IterativeTreeSolver::IterativeTreeSolver(
			const htd::ITreeDecompositionAlgorithm &decomposer,
			std::vector<std::unique_ptr<const ITreeAlgorithm> > &&algorithms,
			std::unique_ptr<const ITreeSolutionExtractor> extractor,
			bool deleteAlgorithms,
			bool deleteExtractor)
		: decomposer_(decomposer),
		  extractor_(extractor.get()),
		  manageAlgorithmMemory_(deleteAlgorithms),
		  manageExtractorMemory_(deleteExtractor)
	{
		for(std::unique_ptr<const ITreeAlgorithm> &alg : algorithms)
			algorithms_.push_back(alg.get());

		for(std::unique_ptr<const ITreeAlgorithm> &alg : algorithms)
			alg.release();
		extractor.release();
	}

	IterativeTreeSolver::IterativeTreeSolver(
			const htd::ITreeDecompositionAlgorithm &decomposer,
			const ITreeAlgorithm &algorithm)
		: IterativeTreeSolver(decomposer, { &algorithm })
	{ }

	IterativeTreeSolver::IterativeTreeSolver(
			const htd::ITreeDecompositionAlgorithm &decomposer,
			const ITreeAlgorithm &algorithm1,
			const ITreeAlgorithm &algorithm2)
		: IterativeTreeSolver(decomposer, { &algorithm1, &algorithm2 })
	{ }

	IterativeTreeSolver::IterativeTreeSolver(
			const htd::ITreeDecompositionAlgorithm &decomposer,
			const TreeAlgorithmVector &algorithms)
		: decomposer_(decomposer),
		  algorithms_(algorithms),
		  extractor_(new NullTreeSolutionExtractor()),
		  manageAlgorithmMemory_(false),
		  manageExtractorMemory_(true)
	{ }

	IterativeTreeSolver::IterativeTreeSolver(
			const htd::ITreeDecompositionAlgorithm &decomposer,
			const ITreeAlgorithm &algorithm,
			const ITreeSolutionExtractor &extractor)
		: IterativeTreeSolver(decomposer, { &algorithm }, extractor)
	{ }

	IterativeTreeSolver::IterativeTreeSolver(
			const htd::ITreeDecompositionAlgorithm &decomposer,
			const ITreeAlgorithm &alg1,
			const ITreeAlgorithm &alg2,
			const ITreeSolutionExtractor &extractor)
		: IterativeTreeSolver(decomposer, { &alg1, &alg2 }, extractor)
	{ }

	IterativeTreeSolver::IterativeTreeSolver(
			const htd::ITreeDecompositionAlgorithm &decomposer,
			const TreeAlgorithmVector &algorithms,
			const ITreeSolutionExtractor &extractor)
		: decomposer_(decomposer),
		  algorithms_(algorithms),
		  extractor_(&extractor),
		  manageAlgorithmMemory_(false),
		  manageExtractorMemory_(false)
	{ }


	IterativeTreeSolver::~IterativeTreeSolver()
	{
		if(manageAlgorithmMemory_)
			for(const ITreeAlgorithm *alg : algorithms_)
				if(alg)
					delete alg;

		if(manageExtractorMemory_ && extractor_) delete extractor_;
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
		unique_ptr<INodeTableMap> tables = 
			this->initializeMap(td.vertexCount());

		bool success = true;
		for(const ITreeAlgorithm *alg : algorithms_)
			if(!(success = this->evaluate(td, *alg, instance, *tables)))
				break;
		Benchmark::registerTimestamp("solving time");

		vertex_t root = td.root();
		ISolution *sol = nullptr;

		if(success)
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

	bool IterativeTreeSolver::evaluate(
			const ITreeDecomposition &td,
			const ITreeAlgorithm &algorithm,
			const IInstance &instance,
			INodeTableMap &tables) const
	{
		bool needAllTables = 
			algorithms_.size() > 1 || algorithm.needAllTables();

		bool finishedBranch = false;
		vertex_t current = td.root();
		ITable *currentTable = nullptr;
		stack<pair<vertex_t, size_t> > parents;

		while(!parents.empty() || !finishedBranch)
		{
			if(!finishedBranch)
			{
				if(td.childCount(current) != 0)
				{
					parents.push(make_pair(current, 1));
					current = td.childAtPosition(current, 0);
					continue;
				}

				parents.push(make_pair(current, 0));
				finishedBranch = true;
			}

			pair<vertex_t, size_t> top = parents.top();
			parents.pop();
			if(td.childCount(top.first) > top.second)
			{
				current = td.childAtPosition(top.first, top.second);
				parents.push(make_pair(top.first, top.second + 1));
				finishedBranch = false;
				continue;
			}
			current = top.first;
			currentTable = algorithm.evaluateNode(
											current,
											td,
											tables,
											instance);

			if(currentTable) 
				insertIntoMap(current, td, currentTable, tables, needAllTables);
			else
				return false;
		}
		
		return true;
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
			INodeTableMap &tables,
			bool needAllTables) const
	{
		IMutableNodeTableMap &map =
			dynamic_cast<IMutableNodeTableMap &>(tables);

		map.insert(node, table);
		
		if(!needAllTables)
		{
			size_t childCount = td.childCount(node);
			for(size_t childIndex = 0; childIndex < childCount; ++childIndex)
				map.erase(td.childAtPosition(node, childIndex));
		}
	}

} // namespace sharp
