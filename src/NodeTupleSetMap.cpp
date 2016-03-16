#ifdef HAVE_CONFIG_H
	#include <config.h>
#endif

#include "NodeTupleSetMap.hpp"

#include <stdexcept>

namespace sharp
{
	using htd::vertex_t;
	
	using std::size_t;

	NodeTupleSetMap::NodeTupleSetMap() : NodeTableMap() { }

	NodeTupleSetMap::NodeTupleSetMap(size_t size) : NodeTableMap(size) { }

	NodeTupleSetMap::~NodeTupleSetMap() { }

	ITupleSet &NodeTupleSetMap::operator[](vertex_t node)
	{
		return static_cast<ITupleSet &>(this->NodeTableMap::operator[](node));
	}
	
	ITupleSet &NodeTupleSetMap::at(vertex_t node)
	{
		return static_cast<ITupleSet &>(this->NodeTableMap::at(node));
	}

	const ITupleSet &NodeTupleSetMap::operator[](vertex_t node) const
	{
		return static_cast<const ITupleSet &>(
				this->NodeTableMap::operator[](node));
	}

	const ITupleSet &NodeTupleSetMap::at(vertex_t node) const
	{
		return static_cast<const ITupleSet &>(
				this->NodeTableMap::at(node));
	}

	bool NodeTupleSetMap::contains(vertex_t node) const
	{
		return this->NodeTableMap::contains(node);
	}

} // namespace sharp
