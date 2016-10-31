#pragma once
#ifndef _ALGS_H
#define _ALGS_H

#include <tuple>

namespace algs
{
	
	template <typename T, std::size_t N>
	T median(const std::array<T, N>& first, const std::array<T, N> second)
	{
		return internal::median_impl(first, internal::bounds{ 0, N }, second, internal::bounds{ 0, N });
	}

	namespace internal
	{
		using bounds = std::pair<std::size_t, std::size_t>;

		template <typename T, std::size_t N, std::size_t M, typename = std::enable_if_t<N + M >= 6>>
		T median_impl(const std::array<T, N>& first, bounds first_bounds, const std::array<T, M>& second, bounds second_bounds)
		{
			auto first_mid = (first_bounds.first + first_bounds.second) / 2 - 1;
			auto second_mid = (second_bounds.first + second_bounds.second) / 2 - 1;
			if (first[first_mid] < second[second_mid])
			{
				//adjust bounds
				return median_impl(first, bounds{ first_mid, first_bounds.second },
					second, bounds{ second_bounds.first, second_mid });
			}
			else
			{
				return median_impl(first, bounds{first_bounds.first, first_mid},
					second, bounds{second_mid, second_bounds.second});
			}
		}

		template <typename T, std::size_t N, std::size_t M, typename = std::enable_if_t<N + M < 6>>
			T median_impl(const std::array<T, N>& first, bounds first_bounds, const std::array<T, M>& second, bounds second_bounds)
			{
				//todo
			}
	}
}

#endif