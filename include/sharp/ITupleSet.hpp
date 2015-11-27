#ifndef SHARP_SHARP_ITUPLESET_H_
#define SHARP_SHARP_ITUPLESET_H_

#include <sharp/global>

#include <sharp/ITable.hpp>
#include <sharp/ITuple.hpp>
#include <sharp/Enumerator.hpp>
#include <sharp/ConstEnumerator.hpp>

#include <utility>
#include <cstddef>

namespace sharp
{
	class SHARP_API ITupleSet : public ITable
	{
	protected:
		ITupleSet &operator=(ITupleSet &) { return *this; };

	public:
		typedef ITuple value_type;
		typedef ITuple &reference;
		typedef const ITuple &const_reference;
		typedef IEnumerator<ITuple>::Iterator iterator;
		typedef IConstEnumerator<ITuple>::Iterator const_iterator;
		typedef std::ptrdiff_t difference_type;
		typedef std::size_t size_type;

		virtual ~ITupleSet() = 0;

		virtual std::pair<iterator, bool> insert(ITuple *tuple) = 0;
		virtual size_type erase(const ITuple &tuple) = 0;
		virtual IEnumerator<ITuple> *enumerate() = 0;
		virtual iterator begin() = 0;
		virtual iterator end() = 0;
		virtual iterator find(const ITuple &tuple) = 0;

		virtual size_type size() const = 0;
		virtual bool contains(const ITuple &tuple) const = 0;
		virtual IConstEnumerator<ITuple> *enumerate() const = 0;
		virtual const_iterator begin() const = 0;
		virtual const_iterator end() const = 0;
		virtual const_iterator find(const ITuple &tuple) const = 0;

	}; // class ITupleSet

	inline ITupleSet::~ITupleSet() { }
} // namespace sharp

#endif // SHARP_SHARP_ITUPLESET_H_
