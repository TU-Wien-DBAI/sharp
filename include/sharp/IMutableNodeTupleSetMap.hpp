#ifndef SHARP_SHARP_IMUTABLENODETUPLESETMAP_H_
#define SHARP_SHARP_IMUTABLENODETUPLESETMAP_H_

#include <sharp/global>

#include <sharp/INodeTupleSetMap.hpp>
#include <sharp/ITupleSet.hpp>

#include <htd/main>

namespace sharp
{
	class SHARP_API IMutableNodeTupleSetMap : public INodeTupleSetMap
	{
	protected:
		IMutableNodeTupleSetMap &operator=(IMutableNodeTupleSetMap &)
		{ return *this; };

	public:
		virtual ~IMutableNodeTupleSetMap() = 0;

		virtual void insert(htd::vertex_t node, ITupleSet *tupleSet) = 0;
		virtual void erase(htd::vertex_t node) = 0;

	}; // class IMutableNodeTupleSetMap

	inline IMutableNodeTupleSetMap::~IMutableNodeTupleSetMap() { }
} // namespace sharp

#endif // SHARP_SHARP_IMUTABLENODETUPLESETMAP_H_
