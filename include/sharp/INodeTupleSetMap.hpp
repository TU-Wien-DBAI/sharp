#ifndef SHARP_SHARP_INODETUPLESETMAP_H_
#define SHARP_SHARP_INODETUPLESETMAP_H_

#include <sharp/global>

#include <sharp/INodeTableMap.hpp>
#include <sharp/ITupleSet.hpp>

#include <htd/main>

namespace sharp
{
	class SHARP_API INodeTupleSetMap : public INodeTableMap
	{
	protected:
		INodeTupleSetMap &operator=(INodeTupleSetMap &) { return *this; };

	public:
		virtual ~INodeTupleSetMap() = 0;

		virtual ITupleSet &operator[](htd::vertex_t node) = 0;
		virtual ITupleSet &at(htd::vertex_t node) = 0;

		virtual const ITupleSet &operator[](htd::vertex_t node) const = 0;
		virtual const ITupleSet &at(htd::vertex_t node) const = 0;

	}; // class INodeTupleSetMap

	inline INodeTupleSetMap::~INodeTupleSetMap() { }
} // namespace sharp

#endif // SHARP_SHARP_INODETUPLESETMAP_H_
