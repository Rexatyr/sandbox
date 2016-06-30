#pragma once
#ifndef CIRCULAR_ITERATOR_H
#define CIRCULAR_ITERATOR_H
#include <type_traits>
#include "custom_type_traits.h"

template <typename T, typename B = std::enable_if_t<is_forward<typename T::iterator_category>()>>
//requires forward_iterator(T)
class circular_iterator
{
	public:
		using value_type = typename T::value_type;
		using difference_type = typename T::difference_type;
		using TThis = circular_iterator<T, B>;
		using reference = typename T::reference;
		using const_reference = const reference;
		using pointer = typename T::pointer;
		using const_pointer = const pointer;
		using iterator_category = typename T::iterator_category;

		friend bool operator==(const TThis&, const TThis&);
		friend difference_type operator-(const TThis&, const TThis&);

	public:
		//cons/dest/assign
		template <typename = std::enable_if_t<std::is_default_constructible<T>::value>>
		circular_iterator() = default;
		circular_iterator(const TThis&) = default;
		template <typename = std::enable_if_t<std::is_move_constructible<T>>, typename = void>
		circular_iterator(TThis&&) = default;
		template <typename = std::enable_if_t<std::conjunction<std::negation<std::is_move_constructible<T>>, std::is_move_assignable<T>, std::is_default_constructible<T>>::value>>
		circular_iterator(TThis&& other) : m_begin(), m_cur(), m_end()
		{
			m_begin = std::move(other.m_begin);
			m_cur = std::move(other.m_cur);
			m_end = std::move(other.m_end);
		}

		template <typename = std::enable_if_t<!is_random_access<iterator_category>()>, typename = void>
		circular_iterator(T p_begin, T p_end, difference_type p_offset = 0) : m_begin(p_begin), m_end(p_end), m_cur(m_begin)
		{
			if (p_offset != 0)
				internal_increment(p_offset);
		}
		template <typename = std::enable_if_t<is_random_access<iterator_category>()>>
		circular_iterator(T p_begin, T p_end, difference_type p_offset = 0) : m_begin(p_begin), m_end(p_end), m_cur(m_begin)
		{
			if (p_offset != 0)
				internal_increment(p_offset);
		}

		circular_iterator(T p_begin, T p_end, T p_cur) : m_begin(p_begin), m_end(p_end), m_cur(p_cur) {}

		~circular_iterator() = default;

		TThis& operator=(const TThis&) = default;
		template <typename = std::enable_if_t<std::is_move_assignable<T>>>
		TThis& operator=(TThis&&) = default;

		void swap(TThis& other)
		{
			using std::swap;
			swap(m_begin, other.m_begin);
			swap(m_cur, other.m_cur);
			swap(m_end, other.m_end);
		}

		//element access
		reference operator*()
		{
			return *m_cur;
		}

		const_reference operator*() const
		{
			return *m_cur;
		}

		pointer operator->()
		{
			return m_cur.operator->();
		}

		const_pointer operator->() const
		{
			return m_cur.operator->();
		}

		//increment
		TThis& operator++()
		{
			++m_cur;
			check_and_reset();
			return *this;
		}

		TThis operator++(int)
		{
			TThis old(*this);
			++m_cur;
			check_and_reset();
			return old;
		}

		//decrement - available if type parameter provides operations
		template <typename = std::enable_if_t<is_bidirectional<iterator_category>()>>
		TThis& operator--()
		{
			check_and_reset();
			--m_cur;
			return *this;
		}

		template <typename = std::enable_if_t<is_bidirectional<iterator_category>()>>
		TThis operator--(int)
		{
			TThis old(*this);
			check_and_reset();
			--m_cur;
			return old;
		}

		//template <typename = std::enable_if_t<std::conjunction<has_compound_addition<T>, has_compound_subtraction<T>>::value>>
		template <typename = std::enable_if_t<is_random_access<iterator_category>()>>
		TThis& operator+=(int diff)
		{
			internal_increment(diff);
			return *this;
		}

		template <typename = std::enable_if_t<is_random_access<iterator_category>()>>
		TThis& operator-=(int diff)
		{
			internal_decrement(diff);
			return *this;
		}

		template <typename = std::enable_if_t<is_random_access<iterator_category>()>>
		reference operator[](int p_index)
		{
			TThis tmp(*this);
			return *(tmp += p_index);
		}

		template <typename = std::enable_if_t<is_random_access<iterator_category>()>>
		const_reference operator[](int p_index) const
		{
			TThis tmp(*this);
			return *(tmp += p_index);
		}

		template <typename = std::enable_if_t<is_random_access<iterator_category>()>>
		bool operator<(const TThis& other) const
		{
			return m_cur < other.m_cur;
		}

		template <typename = std::enable_if_t<is_random_access<iterator_category>()>>
		bool operator<=(const TThis& other) const
		{
			return *this < other || *this == other;
		}

		template <typename = std::enable_if_t<is_random_access<iterator_category>()>>
		bool operator>(const TThis& other) const
		{
			return m_cur > other.m_cur;
		}

		template <typename = std::enable_if_t<is_random_access<iterator_category>()>>
		bool operator>=(const TThis& other)
		{
			return *this > other || other == *this;
		}

	private:
		//check whether iterator is at one of the end points. If it is, it is set to the opposite end.
		//Used to wrap around at the end of the bounds.
		//needs to be called after increment but before decrement (since m_end denotes off-the-end)
		void check_and_reset()
		{
			if (m_cur == m_begin)
			{
				m_cur = m_end;
			}
			else if (m_cur == m_end)
			{
				m_cur = m_begin;
			}
		}

		void internal_increment(difference_type p_diff)
		{
			if (p_diff < 0)
			{
				internal_decrement(-p_diff);
				return;
			}
			auto entire_diff = m_end - m_cur;
			auto cur_diff = m_end - m_cur;
			if (!(p_diff %= entire_diff))
			{
				return;
			}
			if (p_diff >= cur_diff)
			{
				p_diff -= cur_diff;
				m_cur = m_begin;
				std::advance(m_cur, p_diff);
			}
			else
			{
				std::advance(m_cur, p_diff);
			}
			check_and_reset();
		}

		void internal_decrement(difference_type p_diff)
		{
			if (p_diff < 0)
			{
				internal_increment(-p_diff);
				return;
			}
			auto entire_diff = m_end - m_begin;
			auto cur_diff = m_cur - m_begin;
			if (!(p_diff %= entire_diff))
			{
				return;
			}
			if (p_diff >= cur_diff)
			{
				p_diff -= cur_diff;
				m_cur = m_end;
				std::advance(m_cur, -p_diff);
			}
			else
			{
				std::advance(m_cur, -p_diff);
			}
			check_and_reset();
		}

	private:
		T m_begin;
		T m_cur;
		T m_end;
};

template <typename T>
bool operator==(const circular_iterator<T>& lhs, const circular_iterator<T>& rhs)
{
	//bounds comparison is necessary to satisfy forward_iterator's multipass guarantee. Otherwise, 
	//sequentially incrementing both iterators would eventually violate the requirement.
	return (lhs.m_begin == rhs.m_begin) && (lhs.m_cur == rhs.m_cur) && lhs(.m_end == rhs.m_end);
}

template <typename T>
bool operator!=(const circular_iterator<T>& lhs, const circular_iterator<T>& rhs)
{
	return !(lhs == rhs);
}

template <typename T, typename = std::enable_if_t<is_random_access<T::iterator_category>()>>
circular_iterator<T> operator+(circular_iterator<T> lhs, int rhs)
{
	return lhs += rhs;
}

template <typename T, typename = std::enable_if_t<is_random_access<T::iterator_category>()>>
circular_iterator<T> operator+(int lhs, circular_iterator<T> rhs)
{
	return rhs += lhs;
}

template <typename T, typename = std::enable_if_t<is_random_access<T::iterator_category>()>>
circular_iterator<T> operator-(circular_iterator<T> lhs, int rhs)
{
	return lhs -= rhs;
}

template <typename T, typename = std::enable_if_t<is_random_access<T::iterator_category>()>>
typename T::difference_type operator-(const circular_iterator<T>& lhs, const circular_iterator<T>& rhs)
{
	return lhs.m_cur - rhs.m_cur;
}

namespace std
{
	template <typename T>
	void swap(circular_iterator<T>& lhs, circular_iterator<T>& rhs)
	{
		lhs.swap(rhs);
	}
}
#endif