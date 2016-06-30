#pragma once
#ifndef _SFINAE_H
#define _SFINAE_H
#include <type_traits>

template <bool pred, typename T, typename U>
struct ifthenelse
{
	using type = T;
};

template <typename T, typename U>
struct ifthenelse<false, T, U>
{
	using type = U;
};

template <size_t N>
constexpr size_t fib()
{
	return fib<N - 1>() + fib <N - 2>();
}

template <>
constexpr size_t fib<1>()
{
	return 1;
}

template <>
constexpr size_t fib<0>()
{
	return 0;
}

template <size_t N>
struct fib_s
{
	constexpr static size_t value = fib_s<N - 1>().value + fib_s<N - 2>().value;
};

template <>
struct fib_s<1>
{
	constexpr static size_t value = 1;
};

template <>
struct fib_s<0>
{
	constexpr static size_t value = 0;
};

//template <size_t N>
//using fib_c = std::integral_constant<size_t, fib_c<N - 1> +fib_c<N - 2>>;

template <typename T, bool pred, T t, T u>
struct ifthenelse_var
{
	T value = t;
};

template <typename T, T t, T u>
struct ifthenelse_var<T, false, t, u>
{
	T value = u;
};

#endif