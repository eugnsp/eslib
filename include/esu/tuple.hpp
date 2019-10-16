#pragma once
#include <esu/tuple/core.hpp>
#include <esu/tuple/all_any_none.hpp>
#include <esu/tuple/for_each.hpp>
#include <esu/tuple/forward_as_zipped.hpp>
#include <esu/tuple/map.hpp>
#include <esu/tuple/select.hpp>
#include <esu/tuple/to_array.hpp>

namespace esu
{
namespace internal
{
template<std::size_t... is>
std::tuple<std::integral_constant<std::size_t, is>...> make_index_tuple(std::index_sequence<is...>);
}

template<std::size_t size>
using Make_index_tuple = decltype(internal::make_index_tuple(std::make_index_sequence<size>{}));
} // namespace esu
