#ifdef HAVE_CONFIG_H
	#include <config.h>
#endif

#include "TupleSet.hpp"

namespace sharp
{
	using std::unordered_set;
	using std::size_t;
	using std::pair;
	using std::make_pair;

	TupleSet::TupleSet() { }

	TupleSet::~TupleSet()
	{
		for(ITuple *tuple : set_)
			delete tuple;
	}

	pair<TupleSet::iterator, bool> TupleSet::insert(ITuple *tuple)
	{
		auto ret = set_.insert(tuple);
		return make_pair(iterator(new Enum(ret.first, set_.end())), ret.second);
	}

	TupleSet::size_type TupleSet::erase(const ITuple &tuple)
	{
		return set_.erase((ITuple *)&tuple);
	}

	IEnumerator<ITuple> *TupleSet::enumerate()
	{
		return new Enum(set_.begin(), set_.end());
	}

	TupleSet::iterator TupleSet::begin()
	{
		return iterator(this->enumerate());
	}

	TupleSet::iterator TupleSet::end()
	{
		return iterator(new Enum(set_.end()));
	}

	TupleSet::iterator TupleSet::find(const ITuple &tuple)
	{
		return iterator(new Enum(set_.find((ITuple *)&tuple), set_.end()));
	}

	TupleSet::size_type TupleSet::size() const
	{
		return set_.size();
	}

	bool TupleSet::contains(const ITuple &tuple) const
	{
		return set_.find((ITuple *)&tuple) != set_.end();
	}

	IConstEnumerator<ITuple> *TupleSet::enumerate() const
	{
		return new ConstEnum(set_.begin(), set_.end());
	}

	TupleSet::const_iterator TupleSet::begin() const
	{
		return const_iterator(this->enumerate());
	}

	TupleSet::const_iterator TupleSet::end() const
	{
		return const_iterator(new ConstEnum(set_.end()));
	}

	TupleSet::const_iterator TupleSet::find(const ITuple &tuple) const
	{
		return const_iterator(
				new ConstEnum(set_.find((ITuple *)&tuple), set_.end()));
	}

} // namespace sharp
