#pragma once
#include <esf/geometry/function.hpp>
#include <esf/geometry/linestring.hpp>
#include <esf/mesh/mesh2.hpp>
#include <esf/tags.hpp>

#include <stdexcept>
#include <type_traits>
#include <vector>

namespace esf
{
namespace internal
{
template<typename Index_type>
void elements_in_linestring_push_back(const Mesh2::Vertex_view& vertex_view,
									  std::vector<Index_type>& indices)
{
	if constexpr (std::is_same_v<Index_type, Vertex_index>)
		indices.push_back(*vertex_view);
}

template<typename Index_type>
void elements_in_linestring_push_back(const Mesh2::Halfedge_view& halfedge_view,
									  std::vector<Index_type>& indices)
{
	if constexpr (std::is_same_v<Index_type, Halfedge_index>)
		indices.push_back(*halfedge_view);
	if constexpr (std::is_same_v<Index_type, Edge_index>)
		indices.push_back(halfedge_view.edge_index());
}
} // namespace internal

template<typename... Index_types>
void elements_in_linestring(const Mesh2& mesh, const Linestring& linestring,
							std::vector<Index_types>&... indices)
{
	(indices.clear(), ...);

	auto vertex_view = mesh.view(mesh.find_vertex(linestring.first()));
	if (!vertex_view.is_valid())
		throw std::runtime_error("The first linestring point is not a mesh vertex");

	(internal::elements_in_linestring_push_back(vertex_view, indices), ...);

	const Vertex_index first_vertex = *vertex_view;
	Vertex_index prev_vertex = Vertex_index::invalid;

	bool search_next;
	do
	{
		search_next = false;

		auto halfedge_circ = vertex_view.halfedge_in_circ();
		const auto halfedge_first = halfedge_circ;

		do
		{
			const auto vertex_from = halfedge_circ->vertex_index();
			if (vertex_from == prev_vertex || !contains(linestring, halfedge_circ->vertex()))
				continue;

			if (vertex_from == first_vertex)
				break;

			prev_vertex = *vertex_view;
			vertex_view.set_index(vertex_from);

			(internal::elements_in_linestring_push_back(vertex_view, indices), ...);
			(internal::elements_in_linestring_push_back(*halfedge_circ, indices), ...);

			search_next = true;
			break;
		} while (++halfedge_circ != halfedge_first);
	} while (search_next);

	if (mesh.find_vertex(linestring.last()) != *vertex_view)
		throw std::runtime_error("The last linestring point is not a mesh vertex");
}
} // namespace esf
