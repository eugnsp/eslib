#pragma once
#include <esf/forward.hpp>

#include <esu/type_traits.hpp>

#include <cstddef>

namespace esf
{
namespace internal
{
template<class System, std::size_t var_idx>
struct Var_type_w : Var_type_w<typename System::Var_list, var_idx> {};

template<std::size_t var_idx, class... Variables>
struct Var_type_w<Var_list<Variables...>, var_idx>
{
	using Type = esu::Nth<var_idx, Variables...>;
};
}

template<class T, std::size_t var_idx = 0>
using Var_type = typename internal::Var_type_w<T, var_idx>::Type;
} // namespace esf
