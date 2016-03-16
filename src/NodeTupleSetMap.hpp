#ifndef SHARP_NODETUPLESETMAP_H_
#define SHARP_NODETUPLESETMAP_H_

#include "NodeTableMap.hpp"

#include <sharp/IMutableNodeTupleSetMap.hpp>

#include <cstddef>

namespace sharp
{
	class SHARP_LOCAL NodeTupleSetMap : NodeTableMap, public INodeTupleSetMap
	{
	public:
		NodeTupleSetMap();
		NodeTupleSetMap(std::size_t size);

		virtual ~NodeTupleSetMap() override;

		virtual ITupleSet &operator[](htd::vertex_t node) override;
		virtual ITupleSet &at(htd::vertex_t node) override;

		virtual const ITupleSet &operator[](htd::vertex_t node) const override;
		virtual const ITupleSet &at(htd::vertex_t node) const override;

		virtual bool contains(htd::vertex_t node) const override;

	}; // class NodeTupleSetMap

} // namespace sharp

#endif // SHARP_NODETUPLESETMAP_H_
