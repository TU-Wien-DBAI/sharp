#ifdef HAVE_CONFIG_H
	#include <config.h>
#endif

#include <sharp/Hash.hpp>
#include <climits>

namespace sharp
{
	using std::size_t;

	namespace
	{
		const size_t HASH_OFFSET =
			sizeof(size_t) >= 8 ? 1099511628211u :
			(sizeof(size_t) >= 4 ? 16777619u : 0u);

		const size_t HASH_PRIME =
			sizeof(size_t) >= 8 ? 14695981039346656037u :
			(sizeof(size_t) >= 4 ? 2166136261u : 0u);

		const size_t CHUNK_SIZE = CHAR_BIT;
	}
	
	Hash::Hash()
		: hash_(HASH_OFFSET),
		  unorderedXor_(HASH_OFFSET),
		  unorderedSum_(0)
	{ }

	Hash::~Hash() { }

	void Hash::add(size_t data)
	{
		size_t size = sizeof(size_t);

		for(size_t shift = 0; shift < size; ++shift)
			this->add((unsigned char) (data >> (shift * CHUNK_SIZE)));
	}

	void Hash::add(unsigned char data)
	{
		hash_ ^= (size_t)data;
		//FIXME: use optimized version of *= via += and <<
		hash_ *= HASH_PRIME; 
	}

	void Hash::reset()
	{
		hash_ = HASH_OFFSET;
	}

	void Hash::addUnordered(size_t data)
	{
		Hash h;
		h.add(data);
		unorderedXor_ ^= h.get();
		unorderedSum_ += h.get();
	}

	void Hash::addUnordered(unsigned char data)
	{
		Hash h;
		h.add(data);
		unorderedXor_ ^= h.get();
		unorderedSum_ += h.get();
	}

	void Hash::incorporateUnordered()
	{
		this->add(unorderedXor_);
		this->add(unorderedSum_);
		this->resetUnordered();
	}

	void Hash::resetUnordered()
	{
		unorderedXor_ = HASH_OFFSET;
		unorderedSum_ = 0;
	}

	size_t Hash::get() const
	{
		return hash_;
	}

	size_t Hash::getUnordered() const
	{
		Hash h;
		h.add(unorderedXor_);
		h.add(unorderedSum_);
		return h.get();
	}

} // namespace sharp
