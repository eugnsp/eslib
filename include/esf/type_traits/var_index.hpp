#pragma once
#include <esu/type_traits.hpp>

#include <cstddef>

namespace esf
{
template<std::size_t var_idx>
struct Var_index : esu::Index<var_idx> {};
} // namespace esf
