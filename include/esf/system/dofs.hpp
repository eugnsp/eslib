#pragma once
#include <esf/type_traits.hpp>

#include <cstddef>

namespace esf
{
///////////////////////////////////////////////////////////////////////////////////////////////
/** Vertex DoFs */

template<std::size_t var_index, class System>
Vertex_dofs<System, var_index> dofs(const System& system, Var_index<var_index>, Vertex_index vertex)
{
	Vertex_dofs<System, var_index> dofs;
	system.dof_mapper().template vertex_dofs<var_index>(vertex, dofs);
	return dofs;
}

template<std::size_t var_index = 0, class System>
Vertex_dofs<System, var_index> dofs(const System& system, Vertex_index vertex)
{
	Vertex_dofs<System, var_index> dofs;
	system.dof_mapper().template vertex_dofs<var_index>(vertex, dofs);
	return dofs;
}

///////////////////////////////////////////////////////////////////////////////////////////////
/** Edge DoFs */

// TODO
template<std::size_t var_index, class System>
Edge_dofs<System, var_index> dofs(const System& system, Var_index<var_index>, Halfedge_index halfedge)
{
	Edge_dofs<System, var_index> dofs;
	system.dof_mapper().template edge_dofs<var_index>(edge(halfedge), dofs);
	return dofs;
}

// TODO
template<std::size_t var_index = 0, class System>
Edge_dofs<System, var_index> dofs(const System& system, Halfedge_index halfedge)
{
	Edge_dofs<System, var_index> dofs;
	system.dof_mapper().template edge_dofs<var_index>(edge(halfedge), dofs);
	return dofs;
}

///////////////////////////////////////////////////////////////////////////////////////////////
/** Edge DoFs */

template<std::size_t var_index, class System>
Edge_dofs<System, var_index> dofs(const System& system, Var_index<var_index>, Edge_index edge)
{
	Edge_dofs<System, var_index> dofs;
	system.dof_mapper().template edge_dofs<var_index>(edge, dofs);
	return dofs;
}

template<std::size_t var_index = 0, class System>
Edge_dofs<System, var_index> dofs(const System& system, Edge_index edge)
{
	Edge_dofs<System, var_index> dofs;
	system.dof_mapper().template edge_dofs<var_index>(edge, dofs);
	return dofs;
}

///////////////////////////////////////////////////////////////////////////////////////////////
/** Cell DoFs */

template<std::size_t var_index, class System>
auto dofs(const System& system, Var_index<var_index>, const typename System::Mesh::Cell_view& cell)
{
	return system.dof_mapper().template dofs<var_index>(cell);
}

template<std::size_t var_index = 0, class System>
auto dofs(const System& system, const typename System::Mesh::Cell_view& cell)
{
	return system.dof_mapper().template dofs<var_index>(cell);
}
}
