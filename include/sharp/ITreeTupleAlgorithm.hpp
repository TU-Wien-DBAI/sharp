#ifndef SHARP_SHARP_ITREETUPLEALGORITHM_H_
#define SHARP_SHARP_ITREETUPLEALGORITHM_H_

#include <sharp/global>

#include <sharp/ISolution.hpp>
#include <sharp/ITupleSet.hpp>
#include <sharp/INodeTupleSetMap.hpp>
#include <sharp/IDecomposableInstance.hpp>

#include <htd/main.hpp>

#include <vector>

namespace sharp
{
	class ITreeTupleAlgorithm;

	typedef std::vector<const ITreeTupleAlgorithm *> TreeTupleAlgorithmVector;

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
				const IDecomposableInstance &instance,
				ITupleSet &outputTuples) const = 0;

		virtual bool needAllTupleSets() const = 0;

	}; // class ITreeTupleAlgorithm

	inline ITreeTupleAlgorithm::~ITreeTupleAlgorithm() { }
} // namespace sharp

#endif // SHARP_SHARP_ITREETUPLEALGORITHM_H_
