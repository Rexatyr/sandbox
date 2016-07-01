#pragma once
#ifndef MY_TUP_H
#define MY_TUP_H

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
		TThis& operator=(const TThis&) = default;
		TThis& operator=(TThis&&) = default;

	public:
		void swap(TThis& rhs)
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
		TThis& operator=(const TThis&) = default;
		TThis& operator=(TThis&&) = default;
	public:
		void swap(TThis& rhs)
		{
			using std::swap;
			swap(m_elem, rhs.m_elem);
		}
};

template <size_t N, typename T, typename... Ts, typename = std::enable_if_t<N == 0>>
T& t_get(my_tup<T, Ts...>& p_tup)
{
	return p_tup.m_elem;
}

template <size_t N, typename T, typename... Ts, typename = std::enable_if_t<N != 0>, typename = void>
typename type_by_index_t<N, T, Ts...>& t_get(my_tup<T, Ts...>& p_tup)
{
	return t_get<N - 1>(p_tup.m_rest);
}

template <size_t N, typename T, typename... Ts, typename = std::enable_if_t<N != 0>, typename = void>
const type_by_index_t<N, T, Ts...>& ct_get(const my_tup<T, Ts...>& p_tup)
{
	return ct_get<N - 1>(p_tup.m_rest);
}

template <size_t N, typename T, typename... Ts, typename = std::enable_if_t<N == 0>>
const T& ct_get(const my_tup<T, Ts...> p_tup)
{
	return p_tup.m_elem;
}

template <size_t N, typename T, typename... Ts, typename = std::enable_if_t<N == 0>>
T& t_get(const my_tup<T, Ts...>& p_tup)
{
	return p_tup.m_elem;
}

template <size_t N, typename T, typename... Ts, typename = std::enable_if_t<N != 0>, typename = void>
typename type_by_index_t<N, T, Ts...>& t_get(const my_tup<T, Ts...>& p_tup)
{
	return t_get<N - 1>(p_tup.m_rest);
}

template <typename T, typename... Ts>
typename my_tup<T, Ts...> make_my_tup(T p_first, Ts... p_rest)
{
	my_tup<T, Ts...> result{};
	result.m_elem = p_first;
	result.m_rest = make_my_tup(p_rest...);
	return result;
}

template <typename T>
typename my_tup<T> make_my_tup(T p_first)
{
	my_tup<T> result{};
	result.m_elem = p_first;
	return result;
}

template <typename T, typename... Ts>
bool operator==(const my_tup<T, Ts...>& lhs, const my_tup<T, Ts...>& rhs)
{
	if (!(lhs.m_elem == rhs.m_elem))
	{
		return false;
	}
	return lhs.m_rest == rhs.m_rest;
}

template <typename T>
bool operator==(const my_tup<T>& lhs, const my_tup<T>& rhs)
{
	return lhs.m_elem == rhs.m_elem;
}

template <typename T, typename... Ts>
bool operator<(const my_tup<T, Ts...>& lhs, const my_tup<T, Ts...>& rhs)
{
	if (!(lhs.m_elem == rhs.m_elem))
	{
		return false;
	}
	return lhs.m_rest <= rhs.m_rest;
}

template <typename T>
bool operator<(const my_tup<T>& lhs, const my_tup<T>& rhs)
{
	return lhs.m_elem < rhs.m_elem;
}

template <typename T, typename... Ts>
bool operator<=(const my_tup<T, Ts...>& lhs, const my_tup<T, Ts...>& rhs)
{
	if (!(lhs.m_elem <= rhs.m_elem))
	{
		return false;
	}
	return lhs.m_rest <= rhs.m_rest;
}

template <typename T>
bool operator<=(const my_tup<T>& lhs, const my_tup<T>& rhs)
{
	return lhs.m_elem <= rhs.m_elem;
}

template <typename T, typename... Ts>
bool operator!=(const my_tup<T, Ts...>& lhs, const my_tup<T, Ts...>& rhs)
{
	if (!(lhs.m_elem != rhs.m_elem))
	{
		return false;
	}
	return lhs.m_rest != rhs.m_rest;
}

template <typename T>
bool operator!=(const my_tup<T>& lhs, const my_tup<T>& rhs)
{
	return lhs.m_elem != rhs.m_elem;
}

template <typename T, typename... Ts>
bool operator>(const my_tup<T, Ts...>& lhs, const my_tup<T, Ts...>& rhs)
{
	if (!(lhs.m_elem > rhs.m_elem))
	{
		return false;
	}
	return lhs.m_rest > rhs.m_rest;
}

template <typename T>
bool operator>(const my_tup<T>& lhs, const my_tup<T>& rhs)
{
	return lhs.m_elem > rhs.m_elem;
}

template <typename T, typename... Ts>
bool operator>=(const my_tup<T, Ts...>& lhs, const my_tup<T, Ts...>& rhs)
{
	if (!(lhs.m_elem >= rhs.m_elem))
	{
		return false;
	}
	return lhs.m_rest >= rhs.m_rest;
}

template <typename T>
bool operator>=(const my_tup<T>& lhs, const my_tup<T>& rhs)
{
	return lhs.m_elem >= rhs.m_elem;
}

template<typename T, typename... Ts>
constexpr size_t my_tup_size(const my_tup<T, Ts...>& p_tup)
{
	return 1 + my_tup_size(p_tup.m_rest);
}

template <typename T>
constexpr size_t my_tup_size(const my_tup<T>&)
{
	return 1;
}
template <typename T, typename... Ts, typename U, typename... Us>
my_tup<T, Ts..., U, Us...> tuple_katze(const my_tup<T, Ts...>& p_first, const my_tup<U, Us...>& p_second)
{
	const auto tmp = tuple_append(p_first, p_second.m_elem);
	return tuple_katze(tmp, p_second.m_rest);
}

template <typename T, typename... Ts, typename U>
my_tup<T, Ts..., U> tuple_katze(const my_tup<T, Ts...>& p_first, const my_tup<U>& p_second)
{
	return tuple_append(p_first, p_second.m_elem);
}

template <typename T, typename... Ts, typename U>
my_tup<T, Ts..., U> tuple_append(const my_tup<T, Ts...>& p_tup, const U& p_elem)
{
	my_tup<T, Ts..., U> result{};
	result.m_elem = p_tup.m_elem;
	result.m_rest = std::move(tuple_append(p_tup.m_rest, p_elem));
	return result;
}

template <typename T, typename U>
my_tup<T, U> tuple_append(const my_tup<T>& p_tup, const U& p_elem)
{
	my_tup<T, U> result{};
	my_tup<U> result_rest{};
	result_rest.m_elem = p_elem;
	result.m_elem = p_tup.m_elem;
	result.m_rest = std::move(result_rest);
	return result;
}

namespace std
{
	template <typename T, typename... Ts>
	void swap(my_tup<T, Ts...>& lhs, my_tup<T, Ts...>& rhs)
	{
		lhs.swap(rhs);
	}
}

#endif