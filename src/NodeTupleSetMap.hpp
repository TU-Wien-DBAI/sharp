#ifndef SHARP_MUTUPLESETNODETUPLESETMAP_H_
#define SHARP_MUTUPLESETNODETUPLESETMAP_H_

#include "NodeTableMap.hpp"

#include <sharp/IMutableNodeTupleSetMap.hpp>

#include <cstddef>

namespace sharp
{
	class SHARP_LOCAL NodeTupleSetMap : public IMutableNodeTupleSetMap
	{
	public:
		NodeTupleSetMap();
		NodeTupleSetMap(std::size_t size);

		virtual ~NodeTupleSetMap() override;

		virtual ITupleSet &operator[](htd::vertex_t node) override;
		virtual ITupleSet &at(htd::vertex_t node) override;
		virtual void insert(htd::vertex_t node, ITupleSet *tupleSet) override;
		virtual void erase(htd::vertex_t node) override;

		virtual const ITupleSet &operator[](htd::vertex_t node) const override;
		virtual const ITupleSet &at(htd::vertex_t node) const override;

	private:
		NodeTableMap map_;

	}; // class NodeTupleSetMap

} // namespace sharp

#endif // SHARP_MUTUPLESETNODETUPLESETMAP_H_
