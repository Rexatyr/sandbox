#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <vector>
#include <map>
#include <unordered_map>
#include <utility>
#include <memory>
#include <type_traits>
#include <algorithm>
#include "sfinae.h"
#include "circular_iterator.h"
#include <forward_list>
#include <list>
#include "my_tup.h"

template <typename T>
circular_iterator<typename T::iterator> make_circular(T& ref)
{
	return circular_iterator<typename T::iterator>(ref.begin(), ref.end());
}

template <typename T>
circular_iterator<typename T::const_iterator> make_const_circular(const T& p_ref)
{
	return circular_iterator<typename T::const_iterator>(p_ref.begin(), p_ref.end());
}

template <typename T>
circular_iterator<typename T::const_iterator> make_circular(const T& p_ref)
{
	return circular_iterator<typename T::const_iterator>(p_ref.begin(), p_ref.end());
}

template <typename It>
circular_iterator<It> make_circular(It p_beg, It p_end)
{
	return circular_iterator<It>(p_beg, p_end);
}

int main(int argc, char** argv)
{
	std::string str;
	
	std::vector<int> vec{ 3, 6, 2, 7, 9 };

	auto tup = make_my_tup(7, vec);
	const auto i = t_get<0>(tup);
	const auto& vr = t_get<1>(tup);

	std::cout << i << "\n";
	for (const auto& val : vr)
	{
		std::cout << val << " ";
	}

	std::cout << "\n";

	std::cout << my_tup_size(tup);

	std::getline(std::cin, str);
}