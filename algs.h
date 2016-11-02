#pragma once
#ifndef _ALGS_H
#define _ALGS_H

#include <tuple>
#include <iterator>
#include <type_traits>


namespace algs
{
	using bounds = std::pair<std::size_t, std::size_t>;

	//todo return iterator instead?
	template <typename It1, typename It2>
	auto median(It1 first_begin, It1 first_end, It2 second_begin, It2 second_end)
	{
		static_assert(std::is_same<std::iterator_traits<It1>::iterator_category, std::random_access_iterator_tag>::value, "median requires random access iterators");
		static_assert(std::is_same<std::iterator_traits<It2>::iterator_category, std::random_access_iterator_tag>::value, "median requires random access iterators");
		const auto first_length = first_end - first_begin;
		const auto second_length = second_end - second_begin;
		const auto length = first_length + second_length;

		//trivial case (less than 6 elements)
		if (length < 6)
		{
			//search linearly (O(1), because of fixed size)
			auto first_index = first_begin;
			auto second_index = second_begin;
			auto median_rank = (length - 1) / 2;

			for (; median_rank > 0; --median_rank)
			{
				if (*first_index < *second_index)
				{
					++first_index;
				}
				else
				{
					++second_index;
				}
			}
			return (*first_index < *second_index) ? *first_index : *second_index;
		}

		//normal case
		auto first_mid = first_begin;
		std::advance(first_mid, (first_length - 1) / 2);
		auto second_mid = second_begin;
		std::advance(second_mid, (second_length - 1) / 2);

		/*	discard part of the input.
		compare both input array's medians.
		by utilizing transitivity, if one median is less than the other, all values less than it in the same array
		are known to be less than at least half of the overall input values and thus can never be the median of the
		entire input. This can be done on both the lower and upper end respectively, discarding just less than half
		of the input during each iteration.
		This is the same principle as binary search (except values from both ends are excluded rather than the entire
		upper/lower half) */

		if (*first_mid < *second_mid)
		{
			//adjust bounds. If input length is even, new bounds of the second array has to be adjusted more carefully
			//to ensure inputs remain the same length.
			++second_mid;
			if (0 == second_length % 2)
				++second_mid;
			return median(first_mid, first_end, second_begin, second_mid);
		}
		else
		{
			//adjust bounds. If input length is even, new bounds of the first array has to be adjusted more carefully
			//to ensure inputs remain the same length.
			++first_mid;
			if (0 == first_length % 2)
				++first_mid;
			return median(first_begin, first_mid, second_mid, second_end);
		}
	}
}

#endif