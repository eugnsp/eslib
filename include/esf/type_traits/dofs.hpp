#pragma once

#include <cstddef>

namespace esf
{
template<class System, std::size_t var_index>
using Vertex_dofs = typename System::Dof_mapper::template Var_vertex_dofs<var_index>;

template<class System, std::size_t var_index>
using Edge_dofs = typename System::Dof_mapper::template Var_edge_dofs<var_index>;

template<class System, std::size_t var_index>
using Face_dofs = typename System::Dof_mapper::template Var_face_dofs<var_index>;

template<class System, std::size_t var_index>
using Cell_dofs = typename System::Dof_mapper::template Var_cell_dofs<var_index>;
} // namespace esf
