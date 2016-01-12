#ifndef SHARP_TUPLESET_H_
#define SHARP_TUPLESET_H_

#include "ITuple.hpp"

#include <sharp/ITupleSet.hpp>

#include <vector>

namespace sharp
{
	class SHARP_LOCAL TupleSet : public ITupleSet
	{
	public:
		TupleSet();
		virtual ~TupleSet();

		virtual std::pair<iterator, bool> insert(ITuple *tuple);
		virtual size_type erase(const ITuple &tuple);
		virtual IEnumerator<ITuple> *enumerate();
		virtual iterator begin();
		virtual iterator end();
		virtual iterator find(const ITuple &tuple);

		virtual size_type size() const;
		virtual bool contains(const ITuple &tuple) const;
		virtual IConstEnumerator<ITuple> *enumerate() const;
		virtual const_iterator begin() const;
		virtual const_iterator end() const;
		virtual const_iterator find(const ITuple &tuple) const;
		
	private:
		std::vector<ITuple *> set_;

		typedef std::vector<ITuple *>::iterator internal_iterator;
		typedef std::vector<ITuple *>::const_iterator
			internal_const_iterator;
		typedef Enumerator<ITuple, internal_iterator> Enum;
		typedef ConstEnumerator<ITuple, internal_const_iterator> ConstEnum;

	}; // class TupleSet

} // namespace sharp

#endif // SHARP_TUPLESET_H_
