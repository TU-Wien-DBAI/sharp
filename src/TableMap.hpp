#ifndef SHARP_TABLEMAP_H_
#define SHARP_TABLEMAP_H_

#include <sharp/ITableMap.hpp>

#include <htd/main>

#include <unordered_map>

#include <cstdlib>

namespace sharp
{
	class SHARP_LOCAL TableMap : public virtual ITableMap
	{
	protected:
		TableMap &operator=(TableMap &) { return *this; };

	public:
		TableMap();
		TableMap(std::size_t size);

		virtual ~TableMap() override;

		virtual ITable &table(htd::vertex_t node) override;
		virtual void addTable(htd::vertex_t node, ITable *table) override;
		virtual void deleteTable(htd::vertex_t node) override;

		virtual const ITable &table(htd::vertex_t node) const override;

	private:
		std::unordered_map<htd::vertex_t, ITable *> map_;

	}; // class TableMap

} // namespace sharp

#endif // SHARP_TABLEMAP_H_
