#pragma once
#include <cstddef>
#include <utility>

namespace esu
{
template<std::size_t index>
struct Select
{
	template<class Pair>
	decltype(auto) operator()(Pair&& pair) const
	{
		return std::get<index>(std::forward<Pair>(pair));
	}
};

using Select_first = Select<0>;
using Select_second = Select<1>;
}
