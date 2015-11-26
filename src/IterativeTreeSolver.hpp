#ifndef SHARP_ITERATIVETREESOLVER_H_
#define SHARP_ITERATIVETREESOLVER_H_

#include <sharp/IterativeTreeSolver.hpp>

#include <sharp/IMutableNodeTableMap.hpp>

#include <utility>

namespace sharp
{
	class SHARP_LOCAL IterativeTreeSolver::Impl
	{
	public:
		Impl(	const htd::ITreeDecompositionAlgorithm &decomposer,
				const ITreeAlgorithm &algorithm,
				const ITreeSolutionExtractor &extractor);

		virtual ~Impl();

		ISolution *solve(const IInstance &instance) const;

	protected:
		std::unique_ptr<htd::ITreeDecomposition> decompose(
				const IInstance &instance) const;

		virtual std::unique_ptr<IMutableNodeTableMap> initializeMap(
				std::size_t size) const;

		virtual void insertIntoMap(
				htd::vertex_t node,
				const htd::ITreeDecomposition &td,
				ITable *table,
				IMutableNodeTableMap &tableMap) const; 

		std::unique_ptr<IMutableNodeTableMap> evaluate(
				const htd::ITreeDecomposition &td,
				const IInstance &instance) const;

	private:
		const htd::ITreeDecompositionAlgorithm &decomposer_;
		const ITreeAlgorithm &algorithm_;
		const ITreeSolutionExtractor &extractor_;

	}; // class IterativeTreeSolver::Impl

} // namespace sharp

#endif // SHARP_ITERATIVETREESOLVER_H_
