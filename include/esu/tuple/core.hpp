#pragma once
#include <tuple>
#include <type_traits>

namespace esu
{
// Gives the number of elements in a tuple
template<class Tuple>
inline constexpr auto tuple_size = std::tuple_size_v<std::remove_reference_t<Tuple>>;

template<std::size_t i, class Tuple>
using Tuple_element = std::tuple_element_t<i, std::remove_reference_t<Tuple>>;

// Returns an offset from the beginning of a tuple to its member
// (UB is involved, use with care!)
template<std::size_t i, class Tuple>
std::size_t tuple_element_offset()
{
	return reinterpret_cast<std::size_t>(&std::get<i>(*static_cast<Tuple*>(0)));
}

namespace internal
{
template<class Tuple>
inline constexpr auto index_sequence_for_tuple = std::make_index_sequence<tuple_size<Tuple>>{};

template<typename Fn>
constexpr auto forward_with_apply(Fn&& fn)
{
	return [&fn](auto&& tuple) -> decltype(auto) {
		return std::apply(std::forward<Fn>(fn), std::forward<decltype(tuple)>(tuple));
	};
}
} // namespace internal
} // namespace esu
