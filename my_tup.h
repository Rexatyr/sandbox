#pragma once
#ifndef MY_TUP_H
#define MY_TUP_H
#include <utility>

template <size_t N, typename T, typename... Rest>
struct type_by_index : type_by_index<N-1, Rest...> {};

template <typename T, typename... Rest>
struct type_by_index<0, T, Rest...>
{
	using type = typename T;
};

template <size_t N, typename T, typename... Ts>
using type_by_index_t = typename type_by_index<N, T, Ts...>::type;

template <typename T, typename... Ts>
struct my_tup
{
	public:
		T m_elem;
		my_tup<Ts...> m_rest;
	public:

		using TThis = my_tup<T, Ts...>;

		my_tup() = default;
		my_tup(const TThis&) = default;
		my_tup(TThis&&) = default;
		
		my_tup(const T& p_elem, const Ts&... p_rest): m_elem(p_elem), m_rest(p_rest...) {}

		TThis& operator=(const TThis& rhs)
		{
			m_elem = rhs.m_elem;
			m_rest = rhs.m_rest;
			return *this;
		}

		template <typename U, typename... Us>
		TThis& operator=(const my_tup<U, Us...>& rhs)
		{
			m_elem = rhs.m_elem;
			m_rest = rhs.m_rest;
			return *this;
		}

		TThis& operator=(TThis&& rhs)
		{
			m_elem = std::move(rhs.m_elem);
			m_rest = std::move(rhs.m_rest);
			return *this;
		}

		template <typename U, typename... Us, typename = std::enable_if_t<std::is_convertible<U, T>::value>>
		TThis& operator=(my_tup<U, Us...>&& rhs)
		{
			m_elem = std::move(rhs.m_elem);
			m_rest = std::move(rhs.m_rest);
			return *this;
		}

	public:
		void swap(TThis& rhs) noexcept
		{
			using std::swap;
			swap(m_elem, rhs.m_elem);
			swap(m_rest, rhs.m_rest);
		}
};

template <typename T>
struct my_tup<T>
{
	public:
		T m_elem;

	public:
		using TThis = my_tup<T>;

		my_tup() = default;
		my_tup(const TThis&) = default;
		my_tup(TThis&&) = default;
		/*TThis& operator=(const TThis&) = default;
		TThis& operator=(TThis&&) = default;*/

		my_tup(const T& p_elem) : m_elem(p_elem) {}
	public:
		void swap(TThis& rhs) noexcept
		{
			using std::swap;
			swap(m_elem, rhs.m_elem);
		}

		template <typename U>
		TThis& operator=(const my_tup<U>& rhs)
		{
			m_elem = rhs.m_elem;
			return *this;
		}

		TThis& operator=(const TThis& rhs)
		{
			m_elem = rhs.m_elem;
			return *this;
		}

		template <typename U>
		TThis& operator=(my_tup<U>&& rhs)
		{
			m_elem = std::move(rhs.m_elem);
			return *this;
		}

		TThis& operator=(TThis&& rhs)
		{
			m_elem = std::move(rhs.m_elem);
			return *this;
		}
};

template <size_t N, typename T, typename... Ts, typename = std::enable_if_t<N == 0>>
T& t_get(my_tup<T, Ts...>& p_tup) noexcept
{
	return p_tup.m_elem;
}

template <size_t N, typename T, typename... Ts, typename = std::enable_if_t<N != 0>, typename = void>
typename type_by_index_t<N, T, Ts...>& t_get(my_tup<T, Ts...>& p_tup) noexcept
{
	return t_get<N - 1>(p_tup.m_rest);
}

template <size_t N, typename T, typename... Ts, typename = std::enable_if_t<N != 0>, typename = void>
const type_by_index_t<N, T, Ts...>& ct_get(const my_tup<T, Ts...>& p_tup)
{
	return ct_get<N - 1>(p_tup.m_rest);
}

template <size_t N, typename T, typename... Ts, typename = std::enable_if_t<N == 0>>
const T& ct_get(const my_tup<T, Ts...> p_tup) noexcept
{
	return p_tup.m_elem;
}

template <size_t N, typename T, typename... Ts, typename = std::enable_if_t<N == 0>>
T& t_get(const my_tup<T, Ts...>& p_tup) noexcept
{
	return p_tup.m_elem;
}

template <size_t N, typename T, typename... Ts, typename = std::enable_if_t<N != 0>, typename = void>
typename type_by_index_t<N, T, Ts...>& t_get(const my_tup<T, Ts...>& p_tup) noexcept
{
	return t_get<N - 1>(p_tup.m_rest);
}

template <typename... Ts>
typename my_tup<Ts...> make_my_tup(const Ts&... p_elems)
{
	return my_tup<Ts...>(p_elems...);
}

template <typename T, typename... Ts>
inline bool operator==(const my_tup<T, Ts...>& lhs, const my_tup<T, Ts...>& rhs) noexcept
{
	return lhs.m_elem == rhs.m_elem && lhs.m_rest == rhs.m_rest;
}

template <typename T>
inline bool operator==(const my_tup<T>& lhs, const my_tup<T>& rhs) noexcept
{
	return lhs.m_elem == rhs.m_elem;
}

template <typename T, typename... Ts>
inline bool operator<(const my_tup<T, Ts...>& lhs, const my_tup<T, Ts...>& rhs) noexcept
{
	return lhs.m_elem < rhs.m_elem || !(lhs.m_elem >= rhs.m_elem) && lhs.m_rest < rhs.m_rest;
}

template <typename T>
inline bool operator<(const my_tup<T>& lhs, const my_tup<T>& rhs) noexcept
{
	return lhs.m_elem < rhs.m_elem;
}

template <typename T, typename... Ts>
inline bool operator<=(const my_tup<T, Ts...>& lhs, const my_tup<T, Ts...>& rhs) noexcept
{
	return lhs.m_elem <= rhs.m_elem || !(lhs.m_elem > rhs.m_elem) && lhs.m_rest <= rhs.m_rest;
}

template <typename T>
inline bool operator<=(const my_tup<T>& lhs, const my_tup<T>& rhs) noexcept
{
	return lhs.m_elem <= rhs.m_elem;
}

template <typename T, typename... Ts>
inline bool operator!=(const my_tup<T, Ts...>& lhs, const my_tup<T, Ts...>& rhs) noexcept
{
	return lhs.m_elem != rhs.m_elem || lhs.m_rest != rhs.m_rest;
}

template <typename T>
inline bool operator!=(const my_tup<T>& lhs, const my_tup<T>& rhs) noexcept
{
	return lhs.m_elem != rhs.m_elem;
}

template <typename T, typename... Ts>
inline bool operator>(const my_tup<T, Ts...>& lhs, const my_tup<T, Ts...>& rhs) noexcept
{
	return lhs.m_elem > rhs.m_elem || !(lhs.m_elem <= rhs.m_elem) && lhs.m_rest > rhs.m_rest;
}

template <typename T>
inline bool operator>(const my_tup<T>& lhs, const my_tup<T>& rhs) noexcept
{
	return lhs.m_elem > rhs.m_elem;
}

template <typename T, typename... Ts>
inline bool operator>=(const my_tup<T, Ts...>& lhs, const my_tup<T, Ts...>& rhs) noexcept
{
	return lhs.m_elem >= rhs.m_elem || !(lhs.m_elem < rhs.m_elem) && lhs.m_rest >= rhs.m_rest;
}

template <typename T>
bool operator>=(const my_tup<T>& lhs, const my_tup<T>& rhs) noexcept
{
	return lhs.m_elem >= rhs.m_elem;
}

template<typename T, typename... Ts>
constexpr size_t my_tup_size(const my_tup<T, Ts...>& p_tup) noexcept
{
	return 1 + my_tup_size(p_tup.m_rest);
}

template <typename T>
constexpr size_t my_tup_size(const my_tup<T>&)
{
	return 1;
}

template <typename T, typename... Ts, typename U, typename... Us>
constexpr typename my_tup<T, Ts..., U, Us...> my_tup_cat(my_tup<T, Ts...> lhs, my_tup<U, Us...> rhs)
{
	return my_tup_cat_impl(lhs, std::index_sequence_for<T, Ts...>{}, rhs, std::index_sequence_for<U, Us...>{});
}

template <typename T, typename... Ts, size_t... Is, typename U, typename... Us, size_t... Js>
constexpr typename my_tup<T, Ts..., U, Us...> my_tup_cat_impl(my_tup<T, Ts...>& lhs, std::index_sequence<Is...>, my_tup<U, Us...>& rhs, std::index_sequence<Js...>)
{
	return make_my_tup(t_get<Is>(lhs)..., t_get<Js>(rhs)...);
}

template <typename... Ts>
constexpr typename my_tup<Ts&...> my_tup_tie(Ts&... p_elems)
{
	return my_tup<Ts&...>(p_elems...);
}

namespace std
{
	template <typename T, typename... Ts>
	void swap(my_tup<T, Ts...>& lhs, my_tup<T, Ts...>& rhs) noexcept
	{
		lhs.swap(rhs);
	}
}

#endif