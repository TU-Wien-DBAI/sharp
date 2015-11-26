#ifndef SHARP_SHARP_ITUPLESET_H_
#define SHARP_SHARP_ITUPLESET_H_

#include <sharp/global>

#include <sharp/ITable.hpp>
#include <sharp/ITuple.hpp>
#include <sharp/IEnumerator.hpp>
#include <sharp/IConstEnumerator.hpp>

#include <iterator>
#include <cstddef>

namespace sharp
{
	class SHARP_API ITupleSet : public ITable
	{
	protected:
		ITupleSet &operator=(ITupleSet &) { return *this; };

	public:
		virtual ~ITupleSet() = 0;

		typedef IEnumerator<ITuple>::Iterator iterator;
		virtual void insert(ITuple *tuple) = 0;
		virtual void erase(const ITuple &tuple) = 0;
		virtual IEnumerator<ITuple> *enumerate() = 0;
		virtual iterator begin() = 0;
		virtual iterator end() = 0;
		virtual iterator find(const ITuple &tuple) = 0;

		typedef IConstEnumerator<ITuple>::ConstIterator const_iterator;
		virtual std::size_t size() const = 0;
		virtual bool contains(const ITuple &tuple) const = 0;
		virtual IConstEnumerator<ITuple> *enumerate() const = 0;
		virtual const_iterator begin() const = 0;
		virtual const_iterator end() const = 0;
		virtual const_iterator find(const ITuple &tuple) const = 0;

	}; // class ITupleSet

	inline ITupleSet::~ITupleSet() { }
} // namespace sharp

#endif // SHARP_SHARP_ITUPLESET_H_
