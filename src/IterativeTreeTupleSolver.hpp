#ifndef SHARP_ITERATIVETREETUPLESOLVER_H_
#define SHARP_ITERATIVETREETUPLESOLVER_H_

#include "IterativeTreeSolver.hpp"

#include <sharp/global>

#include <sharp/ITreeSolver.hpp>
#include <sharp/IDecomposableInstance.hpp>
#include <sharp/ITreeTupleAlgorithm.hpp>
#include <sharp/ITreeTupleSolutionExtractor.hpp>

#include <htd/main.hpp>

namespace sharp
{
	class SHARP_LOCAL IterativeTreeTupleSolver : public IterativeTreeSolver
	{
		class SHARP_LOCAL TupleToTreeAlgorithmAdapter : public ITreeAlgorithm
		{
		public:
			TupleToTreeAlgorithmAdapter(const ITreeTupleAlgorithm &algorithm);

			virtual ~TupleToTreeAlgorithmAdapter() override;

			virtual std::vector<const htd::ILabelingFunction *>
				preprocessOperations() const override;

			virtual ITable *evaluateNode(
					htd::vertex_t node,
					const htd::ITreeDecomposition &decomposition,
					const INodeTableMap &tables,
					const IDecomposableInstance &instance) const override;

			virtual bool needAllTables() const override;

		private:
			const ITreeTupleAlgorithm &algorithm_;

		}; // class TupleToTreeAlgorithmAdapter

		class SHARP_LOCAL TupleToTreeSolutionExtractorAdapter
			: public ITreeSolutionExtractor
		{
		public:
			TupleToTreeSolutionExtractorAdapter(
					const ITreeTupleSolutionExtractor &extractor);

			virtual ~TupleToTreeSolutionExtractorAdapter() override;

			virtual ISolution *extractSolution(
					htd::vertex_t node,
					const htd::ITreeDecomposition &decomposition,
					const INodeTableMap &tables,
					const IDecomposableInstance &instance) const override;

			virtual ISolution *emptySolution(
					const IDecomposableInstance &instance) const override;

		private:
			const ITreeTupleSolutionExtractor &extractor_;

		}; // class TupleToTreeSolutionExtractorAdapter


	protected:
		IterativeTreeTupleSolver &operator=(IterativeTreeTupleSolver &)
		{ return *this; };

	public:
		IterativeTreeTupleSolver(
				const htd::ITreeDecompositionAlgorithm &decomposer,
				const ITreeTupleAlgorithm &algorithm);

		IterativeTreeTupleSolver(
				const htd::ITreeDecompositionAlgorithm &decomposer,
				const ITreeTupleAlgorithm &algorithm1,
				const ITreeTupleAlgorithm &algorithm2);

		IterativeTreeTupleSolver(
				const htd::ITreeDecompositionAlgorithm &decomposer,
				const TreeTupleAlgorithmVector &algorithms);

		IterativeTreeTupleSolver(
				const htd::ITreeDecompositionAlgorithm &decomposer,
				const ITreeTupleAlgorithm &algorithm,
				const ITreeTupleSolutionExtractor &extractor);

		IterativeTreeTupleSolver(
				const htd::ITreeDecompositionAlgorithm &decomposer,
				const ITreeTupleAlgorithm &algorithm1,
				const ITreeTupleAlgorithm &algorithm2,
				const ITreeTupleSolutionExtractor &extractor);

		IterativeTreeTupleSolver(
				const htd::ITreeDecompositionAlgorithm &decomposer,
				const TreeTupleAlgorithmVector &algorithms,
				const ITreeTupleSolutionExtractor &extractor);

		virtual ~IterativeTreeTupleSolver() override;

	private:
		virtual std::unique_ptr<INodeTableMap> initializeMap(
				std::size_t decompositionNodeCount) const override;

		static std::vector<std::unique_ptr<const ITreeAlgorithm> >
		convertAlgorithmList(const TreeTupleAlgorithmVector &algorithms);

	}; // class IterativeTreeTupleSolver

} // namespace sharp

#endif // SHARP_ITERATIVETREETUPLESOLVER_H_
