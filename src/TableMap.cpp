#ifdef HAVE_CONFIG_H
	#include <config.h>
#endif

#include "TableMap.hpp"

namespace sharp
{
	using htd::vertex_t;
	
	using std::size_t;

	TableMap::TableMap()
	{ }

	TableMap::TableMap(size_t size)
		: map_(size)
	{ }

	TableMap::~TableMap()
	{
		for(auto entry : map_)
			delete entry.second;
	}

	ITable &TableMap::table(vertex_t node)
	{
		//FIXME: what to do if not found?
		auto existingEntry = map_.find(node);
		return *existingEntry->second;
	}

	void TableMap::addTable(vertex_t node, ITable *table)
	{
		auto existingEntry = map_.find(node);
		if(existingEntry != map_.end())
		{
			delete existingEntry->second;
			existingEntry->second = table;
		
		}
		else
		{
			map_[node] = table;
		}
	}

	void TableMap::deleteTable(vertex_t node)
	{
		auto existingEntry = map_.find(node);
		if(existingEntry != map_.end())
		{
			delete existingEntry->second;
			map_.erase(existingEntry);
		}
	}

	const ITable &TableMap::table(vertex_t node) const
	{
		//FIXME: what to do if not found?
		auto existingEntry = map_.find(node);
		return *existingEntry->second;
	}

} // namespace sharp
