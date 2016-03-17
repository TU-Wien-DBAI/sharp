#ifdef HAVE_CONFIG_H
#	include <config.h>
#endif

#include "NodeTupleSetMapOverlay.hpp"

namespace sharp
{
	using htd::vertex_t;
	
	NodeTupleSetMapOverlay::NodeTupleSetMapOverlay(
			const INodeTupleSetMap &original,
			htd::vertex_t node,
			const ITupleSet &replacement)
		: map_(original), node_(node), replacement_(replacement)
	{ }

	NodeTupleSetMapOverlay::~NodeTupleSetMapOverlay() { }

	ITupleSet &NodeTupleSetMapOverlay::operator[](vertex_t)
	{
		throw std::runtime_error("Unimplemented function!");
	}

	ITupleSet &NodeTupleSetMapOverlay::at(vertex_t)
	{
		throw std::runtime_error("Unimplemented function!");
	}

	const ITupleSet &NodeTupleSetMapOverlay::operator[](vertex_t node) const
	{
		if(node == node_)
			return replacement_;
		return map_[node];
	}

	const ITupleSet &NodeTupleSetMapOverlay::at(vertex_t node) const
	{
		if(node == node_)
			return replacement_;
		return map_.at(node);
	}

	bool NodeTupleSetMapOverlay::contains(vertex_t node) const
	{
		return node == node_ || map_.contains(node);
	}

} // namespace sharp
