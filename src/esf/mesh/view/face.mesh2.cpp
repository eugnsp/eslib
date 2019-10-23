#include <esf/index.hpp>
#include <esf/mesh/mesh2.hpp>
#include <esf/tags.hpp>

#include <cassert>

namespace esf
{
Halfedge_index Element_view<Face_tag, Mesh2>::halfedge() const
{
	return mesh_.halfedge_index(index_);
}

auto Element_view<Face_tag, Mesh2>::vertex_circ() const -> Vertex_circ
{
	return Vertex_circ{mesh_, halfedge()};
}

auto Element_view<Face_tag, Mesh2>::halfedge_circ() const -> Halfedge_circ
{
	return Halfedge_circ{mesh_, halfedge()};
}

auto Element_view<Face_tag, Mesh2>::edge_circ() const -> Edge_circ
{
	return Edge_circ{mesh_, halfedge()};
}

auto Element_view<Face_tag, Mesh2>::face_circ() const -> Face_circ
{
	return Face_circ{mesh_, halfedge()};
}

void Element_view<Face_tag, Mesh2>::get_indices(Vertex_indices& vertices) const
{
	auto circ = halfedge_circ();
	vertices[0] = circ->vertex_index();
	vertices[1] = (++circ)->vertex_index();
	vertices[2] = (++circ)->vertex_index();

	assert(++circ == halfedge_circ());
}

void Element_view<Face_tag, Mesh2>::get_indices(Vertex_indices& vertices,
												Halfedge_indices& halfedges) const
{
	auto circ = halfedge_circ();
	vertices[0] = circ->vertex_index();
	halfedges[0] = **circ;
	vertices[1] = (++circ)->vertex_index();
	halfedges[1] = **circ;
	vertices[2] = (++circ)->vertex_index();
	halfedges[2] = **circ;

	assert(++circ == halfedge_circ());
}

void Element_view<Face_tag, Mesh2>::get_indices(Vertex_indices& vertices, Edge_indices& edges) const
{
	auto circ = halfedge_circ();
	vertices[0] = circ->vertex_index();
	edges[0] = edge(**circ);
	vertices[1] = (++circ)->vertex_index();
	edges[1] = edge(**circ);
	vertices[2] = (++circ)->vertex_index();
	edges[2] = edge(**circ);

	assert(++circ == halfedge_circ());
}

void Element_view<Face_tag, Mesh2>::get_indices(Vertex_indices& vertices,
												Edge_with_dir_indices& edges_with_direction) const
{
	auto circ = halfedge_circ();
	vertices[0] = circ->vertex_index();
	edges_with_direction[0] = {edge(**circ), is_first_halfedge(**circ)};
	vertices[1] = (++circ)->vertex_index();
	edges_with_direction[1] = {edge(**circ), is_first_halfedge(**circ)};
	vertices[2] = (++circ)->vertex_index();
	edges_with_direction[2] = {edge(**circ), is_first_halfedge(**circ)};

	assert(++circ == halfedge_circ());
}

void Element_view<Face_tag, Mesh2>::get_indices(Halfedge_index first,
												Vertex_indices& vertices,
												Halfedge_indices& halfedges) const
{
	assert(mesh_.face_index(first) == index_);

	auto circ = Halfedge_circ{mesh_, first};
	vertices[0] = circ->vertex_index();
	halfedges[0] = **circ;
	vertices[1] = (++circ)->vertex_index();
	halfedges[1] = **circ;
	vertices[2] = (++circ)->vertex_index();
	halfedges[2] = **circ;

	assert(**(++circ) == first);
}

auto Element_view<Face_tag, Mesh2>::vertices() const -> Vertices
{
	Vertices vertices;
	auto circ = vertex_circ();
	vertices[0] = circ->vertex();
	vertices[1] = (++circ)->vertex();
	vertices[2] = (++circ)->vertex();

	assert(++circ == vertex_circ());
	return vertices;
}
} // namespace esf
