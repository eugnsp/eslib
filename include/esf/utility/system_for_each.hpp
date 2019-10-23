#pragma once
#include <esf/type_traits.hpp>
#include <esf/utility/var_list_for_each.hpp>

#include <cstddef>
#include <type_traits>

namespace esf
{
template<class System, class Fn>
void for_each_variable(const System& system, Fn fn)
{
	esf::for_each_variable<typename System::Var_list>(
		[&]<std::size_t var_idx>(Var_index<var_idx> var_index)
	{
		auto& var = system.variable(var_index);
		if constexpr (std::is_invocable_v<Fn, Var_index<var_idx>, decltype(var)>)
        	fn(var_index, var);
        else
        	fn(var);
	});
}
}
