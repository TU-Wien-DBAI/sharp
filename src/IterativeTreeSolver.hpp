#ifndef SHARP_ITERATIVETREESOLVER_H_
#define SHARP_ITERATIVETREESOLVER_H_

#include <sharp/global>

#include <sharp/ITreeSolver.hpp>
#include <sharp/ITreeAlgorithm.hpp>
#include <sharp/IDecomposableInstance.hpp>
#include <sharp/ISolution.hpp>
#include <sharp/ITreeSolutionExtractor.hpp>

#include <htd/main.hpp>

#include <memory>

namespace sharp
{
	class IterativeTreeTupleSolver;

	class SHARP_LOCAL IterativeTreeSolver : public ITreeSolver
	{
		friend class IterativeTreeTupleSolver;

	protected:
		IterativeTreeSolver &operator=(IterativeTreeSolver &)
		{ return *this; };

	private:
		IterativeTreeSolver(
				const htd::ITreeDecompositionAlgorithm &decomposer,
				std::vector<std::unique_ptr<const ITreeAlgorithm> > &&
																	algorithms,
				bool deleteAlgorithms);


		IterativeTreeSolver(
				const htd::ITreeDecompositionAlgorithm &decomposer,
				std::vector<std::unique_ptr<const ITreeAlgorithm> > &&
																	algorithms,
				std::unique_ptr<const ITreeSolutionExtractor> extractor,
				bool deleteAlgorithms,
				bool deleteExtractor);

	public:
		IterativeTreeSolver(
				const htd::ITreeDecompositionAlgorithm &decomposer,
				const ITreeAlgorithm &algorithm);

		IterativeTreeSolver(
				const htd::ITreeDecompositionAlgorithm &decomposer,
				const ITreeAlgorithm &algorithm1,
				const ITreeAlgorithm &algorithm2);

		IterativeTreeSolver(
				const htd::ITreeDecompositionAlgorithm &decomposer,
				const TreeAlgorithmVector &algorithms);

		IterativeTreeSolver(
				const htd::ITreeDecompositionAlgorithm &decomposer,
				const ITreeAlgorithm &algorithm,
				const ITreeSolutionExtractor &extractor);

		IterativeTreeSolver(
				const htd::ITreeDecompositionAlgorithm &decomposer,
				const ITreeAlgorithm &algorithm1,
				const ITreeAlgorithm &algorithm2,
				const ITreeSolutionExtractor &extractor);

		IterativeTreeSolver(
				const htd::ITreeDecompositionAlgorithm &decomposer,
				const TreeAlgorithmVector &algorithms,
				const ITreeSolutionExtractor &extractor);

		virtual ~IterativeTreeSolver() override;

		virtual htd::ITreeDecomposition *decompose(
				const IDecomposableInstance &instance) const override;

		virtual ISolution *solve(
				const IDecomposableInstance &instance) const override;

		virtual ISolution *solve(
				const IDecomposableInstance &instance,
				const htd::ITreeDecomposition &decomposition) const override;

	private:
		virtual std::unique_ptr<INodeTableMap> initializeMap(
				std::size_t decompositionNodeCount) const;

		void insertIntoMap(
				htd::vertex_t node,
				const htd::ITreeDecomposition &decomposition,
				ITable *table,
				INodeTableMap &tables,
				bool needAllTables) const;

		bool evaluate(
				const htd::ITreeDecomposition &decomposition,
				const ITreeAlgorithm &algorithm,
				const IDecomposableInstance &instance,
				INodeTableMap &tables) const;
		
		const htd::ITreeDecompositionAlgorithm &decomposer_;
		TreeAlgorithmVector algorithms_;
		const ITreeSolutionExtractor *extractor_;
		bool manageAlgorithmMemory_;
		bool manageExtractorMemory_;

	}; // class IterativeTreeSolver

} // namespace sharp

#endif // SHARP_ITERATIVETREESOLVER_H_
