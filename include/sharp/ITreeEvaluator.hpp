#ifndef SHARP_SHARP_ITREEEVALUATOR_H_
#define SHARP_SHARP_ITREEEVALUATOR_H_

#include <sharp/global>

#include <sharp/ISolution.hpp>
#include <sharp/ITable.hpp>
#include <sharp/ITableMap.hpp>

#include <htd/main>

#include <vector>

namespace sharp
{
	class SHARP_API ITreeEvaluator
	{
	protected:
		ITreeEvaluator &operator=(ITreeEvaluator &) { return *this; };

	public:
		virtual ~ITreeEvaluator() = 0;

		virtual std::vector<const htd::ILabelingFunction *>
			preprocessOperations();

		virtual ITable *evaluateNode(
				htd::vertex_t node,
				const htd::ITreeDecomposition &decomposition,
				const ITableMap &tables,
				const IInstance &instance) const = 0;

		virtual ISolution *calculateSolution(
				htd::vertex_t node,
				const htd::ITreeDecomposition &decomposition,
				const ITableMap &tables,
				const IInstance &instance) const = 0;

	}; // class ITreeEvaluator

	inline ITreeEvaluator::~ITreeEvaluator() { }
} // namespace sharp

#endif // SHARP_SHARP_ITREEEVALUATOR_H_
