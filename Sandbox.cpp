#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>
#include <vector>
#include <utility>
#include <memory>
#include <type_traits>
#include <algorithm>
#include "sfinae.h"
#include "circular_iterator.h"
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

//my_tup<double, int> tst()
//{
//	return make_my_tup(3.1415, 20);
//}

int main(int argc, char** argv)
{
	std::string str;

	double d{ 3.14 };
	int x{ 10 };
	
	auto t2 = make_my_tup(5, 5.1);
	auto t3 = make_my_tup(5);
	//auto t = make_my_tup(5, 10);
	my_tup_tie(x, d) = t2;

	std::cout << x << " " << d;

	std::getline(std::cin, str);
}