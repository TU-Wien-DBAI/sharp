#ifndef SHARP_TEST_MOCK_MOCKTREEEVALUATOR_CPP
#define SHARP_TEST_MOCK_MOCKTREEEVALUATOR_CPP

#ifdef HAVE_CONFIG_H
	#include <config.h>
#endif

#include <sharp/ITreeEvaluator.hpp>

namespace sharp { namespace test {

	class MockTreeEvaluator : public virtual ITreeEvaluator
	{
		virtual ~MockTreeEvaluator() { }

		virtual std::vector<const htd::LabelingFunction *> preprocessOperations()
		{
			return default(std::vector<const htd::LabelingFunction *>);
		}

		virtual ITable *evaluateNode(
				htd::vertex_t node,
				const htd::ITreeDecomposition &decomposition,
				const ITableMap &tables,
				const IInstance &instance) const
		{
			return default(ITable *);
		}

		virtual ISolution *calculateSolution(
				htd::vertex_t node,
				const htd::ITreeDecomposition &decomposition,
				const ITableMap &tables,
				const IInstance &instance) const
		{
			return default(ISolution *);
		}

		virtual ISolution *calculateSolution(const IInstance &instance) const
		{
			return default(ISolution *);
		}
	}

}} // namespace sharp::test

#endif // SHARP_TEST_MOCK_MOCKTREEEVALUATOR_CPP 
