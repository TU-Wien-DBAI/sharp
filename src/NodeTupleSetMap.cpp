#ifdef HAVE_CONFIG_H
	#include <config.h>
#endif

#include "NodeTupleSetMap.hpp"

#include <stdexcept>

namespace sharp
{
	using htd::vertex_t;
	
	using std::size_t;

	NodeTupleSetMap::NodeTupleSetMap() { }

	NodeTupleSetMap::NodeTupleSetMap(size_t size) : map_(size) { }

	NodeTupleSetMap::~NodeTupleSetMap() { }

	ITupleSet &NodeTupleSetMap::operator[](vertex_t node)
	{
		return static_cast<ITupleSet &>(map_[node]);
	}
	
	ITupleSet &NodeTupleSetMap::at(vertex_t node)
	{
		return static_cast<ITupleSet &>(map_.at(node));
	}

	void NodeTupleSetMap::insert(vertex_t node, ITupleSet *tupleSet)
	{
		map_.insert(node, tupleSet);
	}

	void NodeTupleSetMap::erase(vertex_t node)
	{
		map_.erase(node);
	}

	const ITupleSet &NodeTupleSetMap::operator[](vertex_t node) const
	{
		ITable &ret = const_cast<NodeTupleSetMap *>(this)->map_[node];
		return static_cast<ITupleSet &>(ret);
	}

	const ITupleSet &NodeTupleSetMap::at(vertex_t node) const
	{
		ITable &ret = const_cast<NodeTupleSetMap *>(this)->map_.at(node);
		return static_cast<ITupleSet &>(ret);
	}

} // namespace sharp
