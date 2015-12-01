#ifndef SHARP_SHARP_ITREETUPLEALGORITHM_H_
#define SHARP_SHARP_ITREETUPLEALGORITHM_H_

#include <sharp/global>

#include <sharp/ISolution.hpp>
#include <sharp/ITuple.hpp>
#include <sharp/ITupleSet.hpp>
#include <sharp/INodeTupleSetMap.hpp>

#include <htd/main>

#include <vector>

namespace sharp
{
	class SHARP_API ITreeTupleAlgorithm
	{
	protected:
		ITreeTupleAlgorithm &operator=(ITreeTupleAlgorithm &)
		{ return *this; };

	public:
		virtual ~ITreeTupleAlgorithm() = 0;

		virtual std::vector<const htd::ILabelingFunction *>
			preprocessOperations() const = 0;

		virtual void evaluateNode(
				htd::vertex_t node,
				const htd::ITreeDecomposition &decomposition,
				const INodeTupleSetMap &tuples,
				const IInstance &instance,
				ITupleSet &outputTuples) const = 0;

	}; // class ITreeTupleAlgorithm

	inline ITreeTupleAlgorithm::~ITreeTupleAlgorithm() { }
} // namespace sharp

#endif // SHARP_SHARP_ITREETUPLEALGORITHM_H_
