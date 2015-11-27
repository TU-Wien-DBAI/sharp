#ifndef SHARP_ITERATIVETREETUPLESOLVER_H_
#define SHARP_ITERATIVETREETUPLESOLVER_H_

#include "IterativeTreeSolverBase.hpp"

#include <sharp/IterativeTreeTupleSolver.hpp>

#include <memory>

namespace sharp
{
	class SHARP_LOCAL IterativeTreeTupleSolver::Impl
		: public IterativeTreeSolverBase
	{
	public:
		Impl(	const htd::ITreeDecompositionAlgorithm &decomposer,
				const ITreeTupleAlgorithm &algorithm);

		Impl(	const htd::ITreeDecompositionAlgorithm &decomposer,
				const ITreeTupleAlgorithm &algorithm,
				const ITreeTupleSolutionExtractor &extractor);

		virtual ~Impl();

	private:
		virtual std::unique_ptr<htd::ITreeDecomposition> decompose(
				const IInstance &instance) const override;

		virtual std::unique_ptr<INodeTableMap> initializeMap(
				std::size_t decompositionNodeCount) const override;

		virtual void insertIntoMap(
				htd::vertex_t node,
				const htd::ITreeDecomposition &td,
				ITable *table,
				INodeTableMap &tables) const override;

		virtual ITable *evaluateNode(
				htd::vertex_t node,
				const htd::ITreeDecomposition &td,
				const INodeTableMap &tables,
				const IInstance &instance) const override;

		virtual ISolution *extractSolution(
				htd::vertex_t node,
				const htd::ITreeDecomposition &td,
				const INodeTableMap &tables,
				const IInstance &instance) const override;

		virtual ISolution *emptySolution(
				const IInstance &instance) const override;

		const htd::ITreeDecompositionAlgorithm &decomposer_;
		const ITreeTupleAlgorithm &algorithm_;
		const ITreeTupleSolutionExtractor *extractor_;
		bool deleteExtractor_;

	}; // class IterativeTreeTupleSolver::Impl

} // namespace sharp

#endif // SHARP_ITERATIVETREETUPLESOLVER_H_
