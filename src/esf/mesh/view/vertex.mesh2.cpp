#include <esf/geometry.hpp>
#include <esf/mesh/mesh2.hpp>
#include <esf/types.hpp>

namespace esf
{
using V = Element_view<Vertex_tag, Mesh2>;

const Point2& V::vertex() const
{
	return mesh_.vertex(index_);
}

auto V::vertex_circ() const -> Vertex_circ
{
	return Vertex_circ{mesh_, mesh_.halfedge_index(index_)};
}

auto V::halfedge_in_circ() const -> Halfedge_in_circ
{
	return Halfedge_in_circ{mesh_, twin(mesh_.halfedge_index(index_))};
}

auto V::halfedge_out_circ() const -> Halfedge_out_circ
{
	return Halfedge_out_circ{mesh_, mesh_.halfedge_index(index_)};
}

auto V::edge_circ() const -> Edge_circ
{
	return Edge_circ{mesh_, mesh_.halfedge_index(index_)};
}

auto V::face_circ() const -> Face_circ
{
	return Face_circ{mesh_, mesh_.halfedge_index(index_)};
}

Local_index V::n_edges() const
{
	auto he = halfedge_in_circ();
	const auto he_first = he;

	Local_index n = 0;
	do
		++n;
	while (++he != he_first);

	return n;
}

Local_index V::n_faces() const
{
	return n_edges() - is_boundary();
}

// Returns the number of neighbouring faces
// Index n_faces() const;

} // namespace esf
