#ifndef SHARP_SHARP_IENUMERATOR_H_
#define SHARP_SHARP_IENUMERATOR_H_

#include <sharp/global>

#include <iterator>
#include <cstddef>

namespace sharp
{
	template<typename T>
	class SHARP_API IEnumerator
	{
	protected:
		IEnumerator &operator=(IEnumerator &) { return *this; };

	public:
		virtual ~IEnumerator() = 0;

		virtual void next() = 0;

		virtual T &get() const = 0;
		virtual bool valid() const = 0;
		virtual IEnumerator<T> *clone() const = 0;

		virtual bool operator==(const IEnumerator<T> &other) const = 0;

	public:
		class Iterator : public std::iterator<std::forward_iterator_tag, T>
		{
		public:
			Iterator()
			{
				enumerator_ = nullptr;
			}

			Iterator(IEnumerator<T> *enumerator)
			{
				enumerator_ = enumerator;
			}

			Iterator(const Iterator &other)
			{
				enumerator_ = other.enumerator_->clone();
			}

			Iterator &operator=(const Iterator &other)
			{
				delete enumerator_;
				enumerator_ = other.enumerator_->clone();
			}

			bool operator==(const Iterator &other) const
			{
				return *enumerator_ == *other.enumerator_;
			}

			bool operator!=(const Iterator &other) const
			{
				return !(*this == *other);
			}

			T &operator*()
			{
				return enumerator_->get();
			}

			Iterator &operator++()
			{
				enumerator_->next();
				return *this;
			}

			Iterator operator++(int)
			{
				Iterator beforeInc(*this);
				++(*this);
				return beforeInc;
			}

		private:
			IEnumerator<T> *enumerator_;

		}; // class Iterator;

	}; // class IEnumerator

	template<typename T>
	inline IEnumerator<T>::~IEnumerator() { }

} // namespace sharp

#endif // SHARP_SHARP_IENUMERATOR_H_
