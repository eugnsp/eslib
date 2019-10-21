#pragma once
#include <cstddef>

namespace esf
{
template<class System, std::size_t var_idx>
using Vertex_dofs = typename System::Dof_mapper::template Var_vertex_dofs<var_idx>;

template<class System, std::size_t var_idx>
using Edge_dofs = typename System::Dof_mapper::template Var_edge_dofs<var_idx>;

template<class System, std::size_t var_idx>
using Face_dofs = typename System::Dof_mapper::template Var_face_dofs<var_idx>;

template<class System, std::size_t var_idx>
using Cell_dofs = typename System::Dof_mapper::template Var_cell_dofs<var_idx>;
} // namespace esf
