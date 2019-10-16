#pragma once
#include <esf/geometry/linestring.hpp>
#include <esf/mesh/algorithm/linestring.hpp>
#include <esf/mesh/mesh2.hpp>
#include <esf/types.hpp>

#include <esu/iterator.hpp>

#include <tuple>
#include <vector>

namespace esf::internal
{
template<class Element>
class Boundary_cond_base<2, Element>
{
	static_assert(Element::dim == 2);
	static_assert(Element::has_vertex_dofs || Element::has_edge_dofs);

public:
	Boundary_cond_base(const Mesh<2>& mesh, const Linestring& boundary)
	{
		if constexpr (Element::has_vertex_dofs && Element::has_edge_dofs)
			std::tie(vertices_, halfedges_) = vertices_and_halfedges_in_linestring(mesh, boundary);
		else if constexpr (Element::has_vertex_dofs)
			vertices_ = vertices_in_linestring(mesh, boundary);
		else
			halfedges_ = halfedges_in_linestring(mesh, boundary);
	}

	template<class... Args>
	auto vertices(const Args&...) const
	{
		static_assert(Element::has_vertex_dofs);
		return esu::Iterable{vertices_};
	}

	template<class... Args>
	auto halfedges(const Args&...) const
	{
		static_assert(Element::has_edge_dofs);
		return esu::Iterable{halfedges_};
	}

protected:
	std::vector<Vertex_index> vertices_;
	std::vector<Halfedge_index> halfedges_;
};
} // namespace esf::internal
