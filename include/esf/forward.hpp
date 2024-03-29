#pragma once
#include <esf/index.hpp>
#include <esf/tags.hpp>

#include <cstddef>

namespace esf
{
////////////////////////////////////////////////////////////////////////////////
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

////////////////////////////////////////////////////////////////////////////////

template<std::size_t, class Space_dim>
class Quadr;

template<class Element, class Quadr>
class Element_quadr;

template<class... Variables>
struct Var_list;

namespace internal
{
template<class Var_list, class Space_dim>
class Dof_mapper_impl;
} // namespace internal

template<class Var_list>
using Dof_mapper = internal::Dof_mapper_impl<Var_list, typename Var_list::Space_dim>;

template<class Var_list,
		 template<class> class Dof_mapper = Dof_mapper>
class System;
} // namespace esf
