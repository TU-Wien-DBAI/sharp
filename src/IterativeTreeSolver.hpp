#ifndef SHARP_ITERATIVETREESOLVER_H_
#define SHARP_ITERATIVETREESOLVER_H_

#include <sharp/global>

#include <sharp/ITreeSolver.hpp>
#include <sharp/ITreeAlgorithm.hpp>
#include <sharp/IInstance.hpp>
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
				std::unique_ptr<const ITreeAlgorithm> algorithm);


		IterativeTreeSolver(
				const htd::ITreeDecompositionAlgorithm &decomposer,
				std::unique_ptr<const ITreeAlgorithm> algorithm,
				std::unique_ptr<const ITreeSolutionExtractor> extractor);

	public:
		IterativeTreeSolver(
				const htd::ITreeDecompositionAlgorithm &decomposer,
				const ITreeAlgorithm &algorithm);

		IterativeTreeSolver(
				const htd::ITreeDecompositionAlgorithm &decomposer,
				const ITreeAlgorithm &algorithm,
				const ITreeSolutionExtractor &extractor);

		virtual ~IterativeTreeSolver() override;

		virtual htd::ITreeDecomposition *decompose(
				const IInstance &instance) const override;

		virtual ISolution *solve(const IInstance &instance) const override;

		virtual ISolution *solve(
				const IInstance &instance,
				const htd::ITreeDecomposition &decomposition) const override;

	private:
		virtual std::unique_ptr<INodeTableMap> initializeMap(
				std::size_t decompositionNodeCount) const;

		void insertIntoMap(
				htd::vertex_t node,
				const htd::ITreeDecomposition &decomposition,
				ITable *table,
				INodeTableMap &tables) const;

		std::unique_ptr<INodeTableMap> evaluate(
				const htd::ITreeDecomposition &decomposition,
				const IInstance &instance) const;
		
		const htd::ITreeDecompositionAlgorithm &decomposer_;
		std::unique_ptr<const ITreeAlgorithm> algorithm_;
		std::unique_ptr<const ITreeSolutionExtractor> extractor_;
		bool manageExtractorMemory_;
		bool manageAlgorithmMemory_;

	}; // class IterativeTreeSolver

} // namespace sharp

#endif // SHARP_ITERATIVETREESOLVER_H_
