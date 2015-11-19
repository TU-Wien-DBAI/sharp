#ifndef SHARP_SHARP_ITABLEMAP_H_
#define SHARP_SHARP_ITABLEMAP_H_

#include <sharp/global>

#include <sharp/ITable.hpp>

#include <htd/main>

namespace sharp
{
	class SHARP_API ITableMap
	{
	protected:
		ITableMap &operator=(ITableMap &) { return *this; };

	public:
		virtual ~ITableMap() = 0;

		virtual ITable &table(htd::vertex_t node) = 0;
		virtual void addTable(htd::vertex_t node, ITable *table) = 0;
		virtual void deleteTable(htd::vertex_t node) = 0;

		virtual const ITable &table(htd::vertex_t node) const = 0;

	}; // class ITableMap

	inline ITableMap::~ITableMap() { }
} // namespace sharp

#endif // SHARP_SHARP_ITABLEMAP_H_
