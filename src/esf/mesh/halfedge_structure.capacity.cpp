#include <esf/mesh/halfedge_structure.hpp>
#include <esf/types.hpp>

#include <cstddef>

namespace esf::internal
{
Vertex_index Halfedge_structure::n_vertices() const
{
	return static_cast<Vertex_index>(vertices_.size());
}

Halfedge_index Halfedge_structure::n_halfedges() const
{
	return static_cast<Halfedge_index>(halfedges_.size());
}

Edge_index Halfedge_structure::n_edges() const
{
	return edge(n_halfedges());
}

Face_index Halfedge_structure::n_faces() const
{
	return static_cast<Face_index>(faces_.size());
}

auto Halfedge_structure::n_cells() const -> Cell_index
{
	return n_faces();
}

Vertex_index Halfedge_structure::n_elements(Vertex_tag) const
{
	return n_vertices();
}

Halfedge_index Halfedge_structure::n_elements(Halfedge_tag) const
{
	return n_halfedges();
}

Edge_index Halfedge_structure::n_elements(Edge_tag) const
{
	return n_edges();
}

Face_index Halfedge_structure::n_elements(Face_tag) const
{
	return n_faces();
}

auto Halfedge_structure::n_elements(Cell_tag) const -> Cell_index
{
	return n_cells();
}

bool Halfedge_structure::is_empty() const
{
	return vertices_.empty() || halfedges_.empty() || faces_.empty();
}

void Halfedge_structure::reserve(Index n_vertices, Index n_edges, Index n_faces)
{
	vertices_.reserve(n_vertices);
	halfedges_.reserve(2 * n_edges);
	faces_.reserve(n_faces);
}

void Halfedge_structure::shrink()
{
	vertices_.shrink_to_fit();
	halfedges_.shrink_to_fit();
	faces_.shrink_to_fit();
}

// Returns approximate total size of memory in bytes occupied by the data structure
std::size_t Halfedge_structure::memory_size() const
{
	return vertices_.capacity() * sizeof(vertices_[0]) +
		   halfedges_.capacity() * sizeof(halfedges_[0]) + faces_.capacity() * sizeof(faces_[0]);
}
} // namespace esf::internal
