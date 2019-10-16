#include <esf/geometry/algorithm.hpp>
#include <esf/geometry/linestring.hpp>
#include <esf/mesh/algorithm/linestring.hpp>
#include <esf/mesh/mesh2.hpp>
#include <esf/types.hpp>

#include <esu/type_traits.hpp>

#include <stdexcept>
#include <utility>
#include <vector>

namespace esf
{
namespace
{
template<class Vertex_fn, class Halfedge_fn>
void elements_in_linestring(
	const Mesh2& mesh, const Linestring& linestring, Vertex_fn vertex_fn, Halfedge_fn halfedge_fn)
{
	auto vertex_view = mesh.view(mesh.find_vertex(linestring.first()));
	if (!is_valid(*vertex_view))
		throw std::runtime_error("The first linestring point is not a mesh vertex");

	vertex_fn(vertex_view);

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

			vertex_fn(vertex_view);
			halfedge_fn(*halfedge_circ);

			search_next = true;
			break;
		} while (++halfedge_circ != halfedge_first);
	} while (search_next);

	if (mesh.find_vertex(linestring.last()) != *vertex_view)
		throw std::runtime_error("The last linestring point is not a mesh vertex");
}
} // namespace

std::vector<Vertex_index> vertices_in_linestring(const Mesh2& mesh, const Linestring& linestring)
{
	std::vector<Vertex_index> vertices;

	elements_in_linestring(
		mesh, linestring, [&vertices](const auto& vertex) { vertices.push_back(*vertex); },
		esu::Nop_fn{});

	return vertices;
}

std::vector<Halfedge_index> halfedges_in_linestring(const Mesh2& mesh, const Linestring& linestring)
{
	std::vector<Halfedge_index> halfedges;

	elements_in_linestring(mesh, linestring, esu::Nop_fn{},
		[&halfedges](const auto& halfedge) { halfedges.push_back(*halfedge); });

	return halfedges;
}

std::pair<std::vector<Vertex_index>, std::vector<Halfedge_index>>
vertices_and_halfedges_in_linestring(const Mesh2& mesh, const Linestring& linestring)
{
	std::vector<Vertex_index> vertices;
	std::vector<Halfedge_index> halfedges;

	elements_in_linestring(
		mesh, linestring, [&vertices](const auto& vertex) { vertices.push_back(*vertex); },
		[&halfedges](const auto& halfedge) { halfedges.push_back(*halfedge); });

	return {vertices, halfedges};
}

std::pair<std::vector<Vertex_index>, std::vector<Edge_index>> vertices_and_edges_in_linestring(
	const Mesh2& mesh, const Linestring& linestring)
{
	std::vector<Vertex_index> vertices;
	std::vector<Edge_index> edges;

	elements_in_linestring(
		mesh, linestring, [&vertices](const auto& vertex) { vertices.push_back(*vertex); },
		[&edges](const auto& halfedge) { edges.push_back(halfedge.edge_index()); });

	return {vertices, edges};
}
} // namespace esf
