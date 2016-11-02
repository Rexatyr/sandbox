#pragma once
#ifndef _ALGS_H
#define _ALGS_H

#include <tuple>

namespace algs
{
	using bounds = std::pair<std::size_t, std::size_t>;

	template <typename T, std::size_t N>
	inline T median(const std::array<T, N>& first, const std::array<T, N>& second)
	{
		return internal::median_impl(first, bounds{ 0, N }, second, bounds{ 0, N });
	}

	namespace internal
	{
		template <typename T, std::size_t N>
		T median_impl(const std::array<T, N>& first, bounds first_bounds, const std::array<T, N>& second, bounds second_bounds)
		{
			//praise the optimizer for letting me do this without runtime penalty without necessarily using evil preprocessor
			//magic. \[T]/
			const auto first_length = first_bounds.second - first_bounds.first;
			const auto second_length = second_bounds.second - second_bounds.first;
			const auto length = first_bounds.second - first_bounds.first + second_bounds.second - second_bounds.first;

			//trivial case (less than 6 elements)
			if (length < 6)
			{
				//search linearly (O(1), because of fixed size)
				auto first_index = first_bounds.first;
				auto second_index = second_bounds.first;
				auto median_rank = length / 2 - ((length % 2) ? 0 : 1);

				for (; median_rank > 0; --median_rank)
				{
					if (first[first_index] < second[second_index])
					{
						++first_index;
					}
					else
					{
						++second_index;
					}
				}
				return (first[first_index] < second[second_index]) ? first[first_index] : second[second_index];
			}

			//normal case
			const auto first_mid = (first_bounds.first + first_bounds.second - 1) / 2;
			const auto second_mid = (second_bounds.first + second_bounds.second - 1) / 2;
			
			/*	discard part of the input.
				compare both input array's medians.
				by utilizing transitivity, if one median is less than the other, all values less than it in the same array 
				are known to be less than at least half of the overall input values and thus can never be the median of the 
				entire input. This can be done on both the lower and upper end respectively, discarding just less than half
				of the input during each iteration. 
				This is the same principle as binary search (except values from both ends are excluded rather than the entire
				upper/lower half) */

			if (first[first_mid] < second[second_mid])
			{
				//adjust bounds. If input length is even, new bounds of the second array has to be adjusted more carefully
				//to ensure inputs remain the same length.
				return median_impl(first, bounds{ first_mid, first_bounds.second },
					second, bounds{second_bounds.first, second_mid + 1 + ((second_length % 2) ? 0 : 1) });
			}
			else
			{
				//adjust bounds. If input length is even, new bounds of the first array has to be adjusted more carefully
				//to ensure inputs remain the same length.
				return median_impl(first, bounds{first_bounds.first, first_mid + 1 + ((first_length % 2) ? 0 : 1) },
					second, bounds{second_mid, second_bounds.second});
			}
		}
	}
}

#endif