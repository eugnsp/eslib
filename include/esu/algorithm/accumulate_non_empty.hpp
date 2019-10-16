#pragma once
#include <esu/type_traits.hpp>

#include <cassert>
#include <functional>
#include <iterator>
#include <numeric>
#include <utility>

namespace esu
{
// Computes the sum of the elements in the range <[first, last)>
// using the given binary function <op>.
template<typename T = void, typename Input_iterator, class Binary_operation>
auto accumulate_non_empty(Input_iterator first, Input_iterator last, Binary_operation op)
{
	using Value = Non_void_t_or<T, typename std::iterator_traits<Input_iterator>::value_type>;

	assert(first != last);
	Value init = std::move(*first);
	return std::accumulate(++first, last, std::move(init), op);
}

// Computes the sum of the elements in the range <[first, last)>.
template<typename T = void, typename Input_iterator>
auto accumulate_non_empty(Input_iterator first, Input_iterator last)
{
	return accumulate_non_empty<T>(first, last, std::plus<>{});
}
} // namespace esu
