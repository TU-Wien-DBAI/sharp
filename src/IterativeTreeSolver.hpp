#ifndef SHARP_ITERATIVETREESOLVER_H_
#define SHARP_ITERATIVETREESOLVER_H_

#include <sharp/IterativeTreeSolver.hpp>

#include <utility>

namespace sharp
{
	class SHARP_LOCAL IterativeTreeSolver::Impl
	{
	public:
		Impl(	const htd::ITreeDecompositionAlgorithm &decomposer,
				const ITreeEvaluator &evaluator);

		virtual ~Impl();

		ISolution *solve(const IInstance &instance) const;

	protected:
		std::unique_ptr<htd::ITreeDecomposition> decompose(
				const IInstance &instance) const;

		virtual std::unique_ptr<ITableMap> initializeTableMap(
				std::size_t size) const;

		virtual void updateTableMap(
				htd::vertex_t node,
				ITable *table,
				ITableMap &tableMap) const; 

		std::unique_ptr<ITableMap> evaluate(
				const htd::ITreeDecomposition &td,
				const IInstance &instance) const;

	private:
		const htd::ITreeDecompositionAlgorithm &decomposer_;
		const ITreeEvaluator &evaluator_;

	}; // class IterativeTreeSolver::Impl

} // namespace sharp

#endif // SHARP_ITERATIVETREESOLVER_H_
