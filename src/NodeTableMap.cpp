#ifdef HAVE_CONFIG_H
	#include <config.h>
#endif

#include "NodeTableMap.hpp"

#include <stdexcept>

namespace sharp
{
	using htd::vertex_t;
	
	using std::size_t;

	NodeTableMap::NodeTableMap() { }

	NodeTableMap::NodeTableMap(size_t size) : map_(size) { }

	NodeTableMap::~NodeTableMap()
	{
		for(auto entry : map_)
			delete entry.second;
	}

	ITable &NodeTableMap::operator[](vertex_t node)
	{
		return this->at(node);
	}
	
	ITable &NodeTableMap::at(vertex_t node)
	{
		auto existingEntry = map_.find(node);
		if(existingEntry == map_.end())
			throw std::logic_error("No table found for given node.");
		return *existingEntry->second;
	}

	void NodeTableMap::insert(vertex_t node, ITable *table)
	{
		if(!table)
			throw std::invalid_argument("Argument 'table' cannot be null!");

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

	void NodeTableMap::erase(vertex_t node)
	{
		auto existingEntry = map_.find(node);
		if(existingEntry != map_.end())
		{
			delete existingEntry->second;
			map_.erase(existingEntry);
		}
	}

	const ITable &NodeTableMap::operator[](vertex_t node) const
	{
		return this->at(node);
	}

	const ITable &NodeTableMap::at(vertex_t node) const
	{
		return const_cast<NodeTableMap *>(this)->at(node);
	}

	bool NodeTableMap::contains(vertex_t node) const
	{
		auto existingEntry = map_.find(node);
		return existingEntry == map_.end();
	}

} // namespace sharp
