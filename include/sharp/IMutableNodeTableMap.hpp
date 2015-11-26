#ifndef SHARP_SHARP_IMUTABLENODETABLEMAP_H_
#define SHARP_SHARP_IMUTABLENODETABLEMAP_H_

#include <sharp/global>

#include <sharp/INodeTableMap.hpp>
#include <sharp/ITable.hpp>

#include <htd/main>

namespace sharp
{
	class SHARP_API IMutableNodeTableMap : public INodeTableMap
	{
	protected:
		IMutableNodeTableMap &operator=(IMutableNodeTableMap &)
		{ return *this; };

	public:
		virtual ~IMutableNodeTableMap() = 0;

		virtual void insert(htd::vertex_t node, ITable *table) = 0;
		virtual void erase(htd::vertex_t node) = 0;

	}; // class IMutableNodeTableMap

	inline IMutableNodeTableMap::~IMutableNodeTableMap() { }
} // namespace sharp

#endif // SHARP_SHARP_IMUTABLENODETABLEMAP_H_
