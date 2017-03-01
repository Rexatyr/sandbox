#pragma once
#ifndef _IPMERGE_H
#define _IPMERGE_H
#include <iterator>

template <typename It, typename TComp = std::less_t>
void inplace_merge(It p_beg, It p_end, TComp p_comp = TComp{})
{
	using diff_t = std::iterator_traits<It>::difference_type;
	auto sz = std::distance(p_beg, p_end);
	for (diff_t step = 1; step < sz; step *= 2)
	{
		auto fst = p_beg; auto snd = p_beg;
		std::advance(snd, step);
		diff_t high = 2 * step;
		while (high <= sz)
		{
			auto f_sz = step; s_sz = step;
			auto buf = snd;
			while (f_sz > 0 && s_sz > 0)
			{
				if (buf < snd)
				{
					if (p_comp(*fst, *snd))
					{
						--f_sz;
						++fst;
					}
					else
					{
						std::swap(*fst, *snd);
						--s_sz;
						++snd;
					}
				}
				else
				{
					if (p_comp(*buf, *snd))
					{
						std::swap(*buf, *fst);
						++buf;
						--f_sz;
						++fst;
					}
				}
				
			}
		}
	}
}

#endif