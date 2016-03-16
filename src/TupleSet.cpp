#ifdef HAVE_CONFIG_H
	#include <config.h>
#endif

#include "TupleSet.hpp"

namespace sharp
{
	using std::vector;
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
		//FIXME: double inserts
		set_.push_back(tuple);
		return make_pair(iterator(new Enum(
						set_.begin() + set_.size() - 2, set_.end())), true);
	}

	TupleSet::size_type TupleSet::erase(const ITuple &tuple)
	{
		//FIXME: this is inefficient
		size_type ret = 0;
		for(size_t i = 0; i < set_.size(); ++i)
			if(set_[i] == (ITuple *)&tuple)
			{
				set_.erase(set_.begin() + i);
				++ret;
			}

		return ret;
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
		size_t i;
		for(i = 0; i < set_.size(); ++i)
			if(set_[i] == (ITuple *)&tuple)
				break;

		return iterator(new Enum(set_.begin() + i, set_.end()));
	}

	TupleSet::size_type TupleSet::size() const
	{
		return set_.size();
	}

	bool TupleSet::contains(const ITuple &tuple) const
	{
		for(size_t i = 0; i < set_.size(); ++i)
			if(set_[i] == (ITuple *)&tuple)
				return true;
		return false;
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
		size_t i;
		for(i = 0; i < set_.size(); ++i)
			if(set_[i] == (ITuple *)&tuple)
				break;

		return const_iterator(
				new ConstEnum(set_.begin() + i, set_.end()));
	}

} // namespace sharp
