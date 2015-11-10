#ifndef SHARP_SHARP_IHTDNODEEVALUATOR_H_
#define SHARP_SHARP_IHTDNODEEVALUATOR_H_

#include <sharp/global.hpp>

#include <sharp/ITable.hpp>

#include <htd/main>

namespace sharp
{
	class SHARP_API IHtdNodeEvaluator
	{
	protected:
		IHtdNodeEvaluator &operator=(IHtdNodeEvaluator &)
		{ return *this; };

	public:
		virtual ~IHtdNodeEvaluator() = 0;

		virtual ITable *evaluateNode(htd::vertex_t node,
				const htd::IHypertreeDecomposition &htd,
				const std::vector<vertex_t> childNodes,
				const std::vector<const ITable *> childTables,
				const IInstance &instance)
			const = 0;

	}; // class IHtdNodeEvaluator

	inline IHtdNodeEvaluator::~IHtdNodeEvaluator() { }
} // namespace sharp

#endif // SHARP_SHARP_IHTDNODEEVALUATOR_H_
