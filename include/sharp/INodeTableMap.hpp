#ifndef SHARP_SHARP_INODETABLEMAP_H_
#define SHARP_SHARP_INODETABLEMAP_H_

#include <sharp/global>

#include <sharp/ITable.hpp>

#include <htd/main>

namespace sharp
{
	class SHARP_API INodeTableMap
	{
	protected:
		INodeTableMap &operator=(INodeTableMap &) { return *this; };

	public:
		virtual ~INodeTableMap() = 0;

		virtual ITable &operator[](htd::vertex_t node) = 0;
		virtual ITable &at(htd::vertex_t node) = 0;

		virtual const ITable &operator[](htd::vertex_t node) const = 0;
		virtual const ITable &at(htd::vertex_t node) const = 0;

	}; // class INodeTableMap

	inline INodeTableMap::~INodeTableMap() { }
} // namespace sharp

#endif // SHARP_SHARP_INODETABLEMAP_H_
