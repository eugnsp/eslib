#pragma once
#include <esf/forward.hpp>

#include <esu/type_traits.hpp>

#include <cstddef>

namespace esf
{
namespace internal
{
template<class, std::size_t var_idx>
struct Var_type_w;

template<std::size_t var_idx, class... Variables>
struct Var_type_w<Var_list<Variables...>, var_idx>
{
	using Type = esu::Nth<var_idx, Variables...>;
};

template<std::size_t var_idx, class Var_list, template<class> class Dof_mapper>
struct Var_type_w<System<Var_list, Dof_mapper>, var_idx> : Var_type_w<Var_list, var_idx> {};
}

template<class T, std::size_t var_idx = 0>
using Var_type = typename internal::Var_type_w<T, var_idx>::Type;
} // namespace esf
