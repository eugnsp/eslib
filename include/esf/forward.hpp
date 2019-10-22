#pragma once
#include <esf/index.hpp>
#include <esf/tags.hpp>

#include <cstddef>

namespace esf
{
//////////////////////////////////////////////////////////////////////////
/** Mesh-related forward declarations */

template<class Space_dim>
class Mesh;

using Mesh1 = Mesh<Dim1>;
using Mesh2 = Mesh<Dim2>;

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
/** Finite elements */

template<std::size_t order, class Space_dim = Dim2>
class Lagrange;

template<std::size_t order, class Space_dim = Dim2>
class Discontinuous_lagrange;

//////////////////////////////////////////////////////////////////////////

template<std::size_t, class Space_dim>
class Quadr;

template<class Element, class Quadr>
class Element_quadr;

namespace internal
{
template<class Space_dim, class Var_list>
class Dof_mapper;
} // namespace internal

template<class Var_list>
using Dof_mapper = internal::Dof_mapper<typename Var_list::Space_dim, Var_list>;
} // namespace esf
