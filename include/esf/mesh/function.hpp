#pragma once
#include <esf/mesh/mesh2/mesh2.hpp>
#include <esf/types.hpp>

#include <cassert>

namespace esf
{
inline Local_index n_adjacent_edges(const Mesh2& mesh, Vertex_index vertex)
{
	assert(vertex < mesh.n_vertices());

	Local_index n = 0;
	mesh.for_each(
		mesh.halfedge_index(vertex), [&n](auto) { ++n; }, Vertex_out_circ_tag{});

	return n;
}

inline Local_index n_adjacent_faces(const Mesh2& mesh, Vertex_index vertex)
{
	auto n = n_adjacent_edges(mesh, vertex);
	n -= mesh.is_boundary(vertex);
	assert(n > 0);

	return n;
}
} // namespace esf
