#ifndef SHARP_SHARP_HASH_H_
#define SHARP_SHARP_HASH_H_

#include <sharp/global>

#include <cstddef>

namespace sharp
{
	class SHARP_API Hash
	{
	public:
		Hash();
		~Hash();

		void add(size_t data);
		void add(unsigned char data);
		void reset();

		void addUnordered(size_t data);
		void addUnordered(unsigned char data);
		void incorporateUnordered();
		void resetUnordered();

		size_t get() const;
		size_t getUnordered() const;

	private:
		size_t hash_;
		size_t unorderedXor_;
		size_t unorderedSum_;

	}; // class Hash

} // namespace sharp

#endif // SHARP_SHARP_HASH_H_
