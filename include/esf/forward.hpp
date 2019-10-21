#pragma once
#include <esf/index.hpp>

#include <cstddef>

namespace esf
{
//////////////////////////////////////////////////////////////////////////
/** Mesh-related forward declarations */

template<Local_index>
class Mesh;

using Mesh1 = Mesh<1>;
using Mesh2 = Mesh<2>;

template<class Element_tag, class Circ_tag>
class End_circular_iterator
{};

template<class Element_tag, class Mesh>
class Random_access_iterator;

template<class Element_tag, class Circ_tag>
class Circulator_vertex_face;

template<class Element_tag, class Circ_tag>
class Circulator_halfedge_edge;

template<class Element_tag, class Mesh>
class Element_view;

//////////////////////////////////////////////////////////////////////////
template<class Element, std::size_t dim, class... Bnd_conds>
class Var;

//////////////////////////////////////////////////////////////////////////
/** Finite elements */

template<std::size_t order, std::size_t dim = 2>
class Lagrange;

template<std::size_t order, std::size_t dim = 2>
class Discontinuous_lagrange;

//////////////////////////////////////////////////////////////////////////

template<std::size_t, std::size_t dim>
class Quadr;

template<class Element, class Quadr>
class Element_quadr;

template<class System>
class Dof_tools;

namespace internal
{
template<esf::Local_index dim, class Element>
class Boundary_cond_base;

template<esf::Local_index, class Var_list>
class Dof_mapper;
} // namespace internal

template<class Var_list>
using Dof_mapper = internal::Dof_mapper<Var_list::space_dim, Var_list>;
} // namespace esf
