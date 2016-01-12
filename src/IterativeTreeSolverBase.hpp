#ifndef SHARP_ITERATIVETREESOLVERBASE_H_
#define SHARP_ITERATIVETREESOLVERBASE_H_

#include <sharp/ITable.hpp>
#include <sharp/INodeTableMap.hpp>
#include <sharp/ISolution.hpp>
#include <sharp/IInstance.hpp>

#include <htd/main.hpp>

#include <memory>

namespace sharp
{
	class SHARP_LOCAL IterativeTreeSolverBase
	{
	public:
		IterativeTreeSolverBase();

		virtual ~IterativeTreeSolverBase();

		std::size_t calculateTreewidth(const IInstance &instance) const;

		ISolution *solve(const IInstance &instance) const;

	private:
		std::unique_ptr<INodeTableMap> evaluate(
				const htd::ITreeDecomposition &td,
				const IInstance &instance) const;

		virtual std::unique_ptr<htd::ITreeDecomposition> decompose(
				const IInstance &instance) const = 0;

		virtual std::unique_ptr<INodeTableMap> initializeMap(
				std::size_t decompositionNodeCount) const = 0;

		virtual void insertIntoMap(
				htd::vertex_t node,
				const htd::ITreeDecomposition &td,
				ITable *table,
				INodeTableMap &tables) const = 0;

		virtual ITable *evaluateNode(
				htd::vertex_t node,
				const htd::ITreeDecomposition &td,
				const INodeTableMap &tables,
				const IInstance &instance) const = 0;

		virtual ISolution *extractSolution(
				htd::vertex_t node,
				const htd::ITreeDecomposition &td,
				const INodeTableMap &tables,
				const IInstance &instance) const = 0;

		virtual ISolution *emptySolution(const IInstance &instance) const = 0;

	}; // class IterativeTreeSolverBase

} // namespace sharp

#endif // SHARP_ITERATIVETREESOLVERBASE_H_
