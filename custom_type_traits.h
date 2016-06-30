#pragma once
#ifndef CUSTOM_TTRAITS_H
#define CUSTOM_TTRAITS_H
#include <type_traits>
#include <iterator>

//#define _IT_STRUCTS

//TODO
template <typename T>
struct is_prefix_decrementable : std::true_type {};

template <typename T>
struct is_postfix_decrementable : std::true_type {};

template <typename T>
struct has_compound_addition : std::true_type {};

template <typename T>
struct has_compound_subtraction : std::true_type {};

template <typename T, typename U>
struct is_addable : std::true_type {};

template <typename T, typename U>
struct is_subtractable : std::true_type {};

#ifdef _IT_STRUCTS

template <typename T>
struct is_random_access_impl : std::false_type {};

template <>
struct is_random_access_impl<std::random_access_iterator_tag> : std::true_type {};

template <typename T>
using is_random_access = is_random_access_impl<T>;

template <typename T>
struct is_forward_impl : std::false_type {};

template <>
struct is_forward_impl<std::forward_iterator_tag> : std::true_type {};

template <>
struct is_forward_impl<std::bidirectional_iterator_tag> : std::true_type {};

template <>
struct is_forward_impl<std::random_access_iterator_tag> : std::true_type {};

template <typename T>
using is_forward = is_forward_impl<T>;

template <typename T>
struct is_bidirectional_impl : std::false_type {};

template <>
struct is_bidirectional_impl<std::random_access_iterator_tag> : std::true_type {};

template <>
struct is_bidirectional_impl<std::bidirectional_iterator_tag> : std::true_type {};

template <typename T>
using is_bidirectional = is_bidirectional_impl<T>;

#else
template <typename T>
constexpr bool is_random_access()
{
	return false;
}

template<>
constexpr bool is_random_access<std::random_access_iterator_tag>()
{
	return true;
}

template <typename T>
constexpr bool is_bidirectional()
{
	return false;
}

template <>
constexpr bool is_bidirectional<std::bidirectional_iterator_tag>()
{
	return true;
}

template <>
constexpr bool is_bidirectional<std::random_access_iterator_tag>()
{
	return true;
}

template <typename T>
constexpr bool is_forward()
{
	return false;
}

template <>
constexpr bool is_forward<std::forward_iterator_tag>()
{
	return true;
}

template <>
constexpr bool is_forward<std::bidirectional_iterator_tag>()
{
	return true;
}

template <>
constexpr bool is_forward<std::random_access_iterator_tag>()
{
	return true;
}
#endif
#endif