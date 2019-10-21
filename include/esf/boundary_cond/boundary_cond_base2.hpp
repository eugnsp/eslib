#pragma once
#include <esf/geometry/linestring.hpp>
#include <esf/mesh/algorithm/linestring.hpp>
#include <esf/mesh/mesh2.hpp>

#include <esu/iterator.hpp>

#include <vector>

namespace esf::internal
{
template<class Element>
class Boundary_cond_base<Dim2, Element>
{
	static_assert(is_dim2<Element>);
	static_assert(Element::has_vertex_dofs || Element::has_edge_dofs);

public:
	static constexpr bool is_strong = true;
	static constexpr bool is_uniform = false;

	using Boundary = Linestring;

public:
	Boundary_cond_base(const Mesh<Dim2>& mesh)
	{
		if constexpr (Element::has_vertex_dofs)
		{
			auto circ = mesh.boundary_vertex_circ();
			const auto first = circ;
			do
				vertices_.push_back(**circ);
			while (++circ != first);
		}

		if constexpr (Element::has_edge_dofs)
		{
			auto circ = mesh.boundary_halfedge_circ();
			const auto first = circ;
			do
				halfedges_.push_back(**circ);
			while (++circ != first);
		}
	}

	Boundary_cond_base(const Mesh<Dim2>& mesh, const Boundary& boundary)
	{
		if constexpr (Element::has_vertex_dofs && Element::has_edge_dofs)
			elements_in_linestring(mesh, boundary, vertices_, halfedges_);
		else if constexpr (Element::has_vertex_dofs)
			elements_in_linestring(mesh, boundary, vertices_);
		else
			elements_in_linestring(mesh, boundary, halfedges_);
	}

	template<class... Ignore>
	auto vertices(Ignore&&...) const
	{
		static_assert(Element::has_vertex_dofs);
		return esu::Iterable{vertices_};
	}

	template<class... Ignore>
	auto halfedges(Ignore&&...) const
	{
		static_assert(Element::has_edge_dofs);
		return esu::Iterable{halfedges_};
	}

protected:
	std::vector<Vertex_index> vertices_;
	std::vector<Halfedge_index> halfedges_;
};
} // namespace esf::internal
