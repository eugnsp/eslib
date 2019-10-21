#pragma once
#include <esf/type_traits.hpp>

#include <cstddef>
#include <type_traits>

namespace esf
{
template<std::size_t var_idx = 0,
		 class System,
		 class Mesh_element_index,
		 typename = std::enable_if_t<is_mesh_element_index<Mesh_element_index>>>
auto dofs(const System& system, Mesh_element_index index, Var_index<var_idx> var_index = {})
{
	return system.dof_mapper().dofs(index, var_index);
}

template<std::size_t var_idx = 0, class System>
auto dofs(const System& system,
		  const typename System::Mesh::Cell_view& cell,
		  Var_index<var_idx> var_index = {})
{
	return system.dof_mapper().dofs(cell, var_index);
}
} // namespace esf
