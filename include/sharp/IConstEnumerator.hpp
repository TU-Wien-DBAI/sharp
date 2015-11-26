#ifndef SHARP_SHARP_ICONSTENUMERATOR_H_
#define SHARP_SHARP_ICONSTENUMERATOR_H_

#include <sharp/global>

#include <iterator>
#include <cstddef>

namespace sharp
{
	template<typename T>
	class SHARP_API IConstEnumerator
	{
	protected:
		IConstEnumerator &operator=(IConstEnumerator &) { return *this; };

	public:
		virtual ~IConstEnumerator() = 0;

		virtual void next() = 0;

		virtual const T &cget() const = 0;
		virtual bool valid() const = 0;
		virtual IConstEnumerator<T> *clone() const = 0;

		virtual bool operator==(const IConstEnumerator<T> &other) const = 0;

	public:
		class ConstIterator
			: public std::iterator< std::forward_iterator_tag,
									T,
									std::ptrdiff_t,
									const T&,
									const T*>
		{
		public:
			ConstIterator()
			{
				enumerator_ = nullptr;
			}

			ConstIterator(IConstEnumerator<T> *enumerator)
			{
				enumerator_ = enumerator;
			}

			ConstIterator(const ConstIterator &other)
			{
				enumerator_ = other.enumerator_->clone();
			}

			ConstIterator &operator=(const ConstIterator &other)
			{
				delete enumerator_;
				enumerator_ = other.enumerator_->clone();
			}

			bool operator==(const ConstIterator &other) const
			{
				return *enumerator_ == *other.enumerator_;
			}

			bool operator!=(const ConstIterator &other) const
			{
				return !(*this == *other);
			}

			const T &operator*()
			{
				return enumerator_->cget();
			}

			ConstIterator &operator++()
			{
				enumerator_->next();
				return *this;
			}

			ConstIterator operator++(int)
			{
				ConstIterator beforeInc(*this);
				++(*this);
				return beforeInc;
			}

		private:
			IConstEnumerator<T> *enumerator_;

		}; // class ConstIterator

	}; // class IConstEnumerator

	template<typename T>
	inline IConstEnumerator<T>::~IConstEnumerator() { }

} // namespace sharp

#endif // SHARP_SHARP_ICONSTENUMERATOR_H_
