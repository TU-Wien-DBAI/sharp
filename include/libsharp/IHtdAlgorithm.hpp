#ifndef SHARP_LIBSHARP_IHTDALGORITHM_H_
#define SHARP_LIBSHARP_IHTDALGORITHM_H_

#include <libsharp/global.hpp>

#include <libsharp/IInstance.hpp>
#include <libsharp/ITupleIterator.hpp>
#include <libsharp/ITuple.hpp>

#include <htdecomp>

namespace sharp
{
	using htd::vertex_t;
	using htd::ITreeDecomposition;

	class SHARP_API IHtdAlgorithm 
	{
	protected:
		IHtdAlgorithm &operator=(const IHtdAlgorithm &) { return *this; }

	public:
		virtual ~IHtdAlgorithm() = 0;

		virtual int getTelemetry(const char name[]) const = 0;
		virtual int getTelemetry(const char name[], vertex_t v, const ITreeDecomposition &decomposition) const = 0;

		virtual ITupleIterator *computeLeaf(vertex_t node, const ITreeDecomposition &decomposition, const IInstance &instance) const = 0;
		virtual ITupleIterator *computeTransform(const ITuple &childTuple, vertex_t node, vertex_t child, const ITreeDecomposition &decomposition, const IInstance &instance) const = 0;
		virtual ITupleIterator *computeJoin(const ITuple &firstTuple, const ITuple &secondTuple, vertex_t node, vertex_t firstChild, vertex_t secondChild, const ITreeDecomposition &decomposition, const IInstance &instance) const = 0;
		virtual ITupleIterator *selectRootSolutions(const ITupleIterator &tuples, const ITreeDecomposition &decomposition, const IInstance &instance) const = 0;

		virtual ITupleIterator *computeTopDown(const ITuple &parentTuple, vertex_t node, vertex_t parent, const ITreeDecomposition &decomposition, const IInstance &instance) const = 0;

	}; // class IHtdAlgorithm

	inline IHtdAlgorithm::~IHtdAlgorithm() { }
} // namespace sharp

#endif // SHARP_LIBSHARP_IHTDALGORITHM_H_
