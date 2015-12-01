#if defined(SHARP_ENUM_CONST) || defined(SHARP_ENUM_NOCONST)
#if !defined(SHARP_ENUM_CONST) || !defined(SHARP_ENUM_NOCONST)

#if defined(SHARP_ENUM_CONST)
	#define SHARP_ENUM_IFACE IConstEnumerator
	#define SHARP_ENUM_NAME ConstEnumerator
	#define SHARP_ENUM_REFTYPE const T &
	#define SHARP_ENUM_PTRTYPE const T *
#elif defined(SHARP_ENUM_NOCONST)
	#define SHARP_ENUM_IFACE IEnumerator
	#define SHARP_ENUM_NAME Enumerator
	#define SHARP_ENUM_REFTYPE T &
	#define SHARP_ENUM_PTRTYPE T *
#else
	#error "Must define either SHARP_ENUM_CONST or SHARP_ENUM_NOCONST"
#endif // SHARP_ENUM_NOCONST

#include <sharp/global>

#include <iterator>
#include <stdexcept>
#include <typeinfo>
#include <cstddef>

namespace sharp
{
	template<typename T>
	class SHARP_API SHARP_ENUM_IFACE
	{
	protected:
		SHARP_ENUM_IFACE &operator=(SHARP_ENUM_IFACE &) { return *this; };

	public:
		virtual ~SHARP_ENUM_IFACE() = 0;

		virtual void next() = 0;

		virtual SHARP_ENUM_REFTYPE get() const = 0;
		virtual bool valid() const = 0;
		virtual SHARP_ENUM_IFACE<T> *clone() const = 0;

		virtual bool operator==(const SHARP_ENUM_IFACE<T> &other) const = 0;

	public:
		class SHARP_API Iterator
			: public std::iterator< std::forward_iterator_tag,
									T,
									std::ptrdiff_t,
									SHARP_ENUM_REFTYPE,
									SHARP_ENUM_PTRTYPE>
		{
		public:
			Iterator()
			{
				enumerator_ = nullptr;
			}

			Iterator(SHARP_ENUM_IFACE<T> *enumerator)
			{
				enumerator_ = enumerator;
			}

			Iterator(const Iterator &other)
				: enumerator_(other.enumerator_->clone())
			{ }

			~Iterator()
			{
				if(enumerator_) delete enumerator_;
			}

			Iterator &operator=(const Iterator &other)
			{
				if(enumerator_) delete enumerator_;
				enumerator_ = other.enumerator_->clone();
			}

			bool operator==(const Iterator &other) const
			{
				return (enumerator_ == nullptr 
					&& other.enumerator_ == nullptr)
					|| (enumerator_ != nullptr 
					&& other.enumerator_ != nullptr
					&& *enumerator_ == *other.enumerator_);
			}

			bool operator!=(const Iterator &other) const
			{
				return !(*this == other);
			}

			SHARP_ENUM_REFTYPE operator*()
			{
				if(!enumerator_)
					throw std::logic_error("Not in dereferenceable state!");
				return enumerator_->get();
			}

			Iterator &operator++()
			{
				if(enumerator_) enumerator_->next();
				return *this;
			}

			Iterator operator++(int)
			{
				Iterator beforeInc(*this);
				++(*this);
				return beforeInc;
			}

		private:
			SHARP_ENUM_IFACE<T> *enumerator_;

		}; // class Iterator;

	}; // class SHARP_ENUM_IFACE

	template<typename T, typename Iter>
	class SHARP_API SHARP_ENUM_NAME : public SHARP_ENUM_IFACE<T>
	{
	public:
		SHARP_ENUM_NAME() { ended_ = true; }

		SHARP_ENUM_NAME(Iter end) : current_(end), end_(end), ended_(true) { }

		SHARP_ENUM_NAME(Iter begin, Iter end)
			: current_(begin), end_(end), ended_(begin == end)
		{ }

		virtual ~SHARP_ENUM_NAME() { }

		SHARP_ENUM_NAME &operator=(const SHARP_ENUM_NAME &other)
		{
			current_ = other.current_;
			end_ = other.end_;
			ended_ = other.ended_;
		}
		
		virtual void next()
		{
			++current_;
			ended_ |= current_ == end_;
		}

		virtual SHARP_ENUM_REFTYPE get() const
		{
			retref<underlying_type> rr;
			return rr(current_);
		}

		virtual bool valid() const
		{
			return !ended_;
		}

		virtual SHARP_ENUM_IFACE<T> *clone() const
		{
			return new SHARP_ENUM_NAME<T, Iter>(*this);
		}

		virtual bool operator==(const SHARP_ENUM_IFACE<T> &other) const
		{
			if(typeid(other) != typeid(*this)) return false;

			const SHARP_ENUM_NAME<T, Iter> &tmpother =
				static_cast<const SHARP_ENUM_NAME<T, Iter> &>(other);
			return *this == tmpother;
		}

		virtual bool operator==(const SHARP_ENUM_NAME<T, Iter> &other) const
		{
			return current_ == other.current_ && end_ == other.end_;
		}

	private:
		typedef typename std::iterator_traits<Iter>::value_type underlying_type;

		Iter current_;
		Iter end_;
		bool ended_;

		template<typename U>
		struct retref { U &operator()(Iter i) { return *i; } };

		template<typename U>
		struct retref<U *> { U &operator()(Iter i) { return **i; } };

	}; // class SHARP_ENUM_NAME
	
	template<typename T>
	inline SHARP_ENUM_IFACE<T>::~SHARP_ENUM_IFACE() { }

} // namespace sharp

#undef SHARP_ENUM_IFACE
#undef SHARP_ENUM_NAME
#undef SHARP_ENUM_REFTYPE
#undef SHARP_ENUM_PTRTYPE

#endif // !defined(SHARP_ENUM_CONST) || !defined(SHARP_ENUM_NOCONST)
#endif // defined(SHARP_ENUM_CONST) || defined(SHARP_ENUM_NOCONST)
