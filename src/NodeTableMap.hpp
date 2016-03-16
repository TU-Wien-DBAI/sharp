#ifndef SHARP_NODETABLEMAP_H_
#define SHARP_NODETABLEMAP_H_

#include <sharp/IMutableNodeTableMap.hpp>

#include <unordered_map>
#include <cstddef>

namespace sharp
{
	class SHARP_LOCAL NodeTableMap : public IMutableNodeTableMap
	{
	public:
		NodeTableMap();
		NodeTableMap(std::size_t size);

		virtual ~NodeTableMap() override;

		virtual ITable &operator[](htd::vertex_t node) override;
		virtual ITable &at(htd::vertex_t node) override;
		virtual void insert(htd::vertex_t node, ITable *table) override;
		virtual void erase(htd::vertex_t node) override;

		virtual const ITable &operator[](htd::vertex_t node) const override;
		virtual const ITable &at(htd::vertex_t node) const override;

		virtual bool contains(htd::vertex_t node) const override;

	private:
		std::unordered_map<htd::vertex_t, ITable *> map_;

	}; // class NodeTableMap

} // namespace sharp

#endif // SHARP_NODETABLEMAP_H_
