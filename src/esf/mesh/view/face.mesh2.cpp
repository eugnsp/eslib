#include <esf/mesh/mesh2.hpp>
#include <esf/types.hpp>

#include <cassert>

namespace esf
{
using V = Element_view<Face_tag, Mesh2>;

Halfedge_index V::halfedge() const
{
	return mesh_.halfedge_index(index_);
}

auto V::vertex_circ() const -> Vertex_circ
{
	return Vertex_circ{mesh_, halfedge()};
}

auto V::halfedge_circ() const -> Halfedge_circ
{
	return Halfedge_circ{mesh_, halfedge()};
}

auto V::edge_circ() const -> Edge_circ
{
	return Edge_circ{mesh_, halfedge()};
}

auto V::face_circ() const -> Face_circ
{
	return Face_circ{mesh_, halfedge()};
}

void V::get_indices(Vertex_indices& vertices) const
{
	auto halfedge = halfedge_circ();
	vertices[0] = halfedge->vertex_index();
	vertices[1] = (++halfedge)->vertex_index();
	vertices[2] = (++halfedge)->vertex_index();

	assert(++halfedge == halfedge_circ());
}

void V::get_indices(Vertex_indices& vertices, Halfedge_indices& halfedges) const
{
	auto halfedge = halfedge_circ();
	vertices[0] = halfedge->vertex_index();
	halfedges[0] = **halfedge;
	vertices[1] = (++halfedge)->vertex_index();
	halfedges[1] = **halfedge;
	vertices[2] = (++halfedge)->vertex_index();
	halfedges[2] = **halfedge;

	assert(++halfedge == halfedge_circ());
}

void V::get_indices(Vertex_indices& vertices, Edge_indices& edges) const
{
	auto halfedge = halfedge_circ();
	vertices[0] = halfedge->vertex_index();
	edges[0] = edge(**halfedge);
	vertices[1] = (++halfedge)->vertex_index();
	edges[1] = edge(**halfedge);
	vertices[2] = (++halfedge)->vertex_index();
	edges[2] = edge(**halfedge);

	assert(++halfedge == halfedge_circ());
}

void V::get_indices(Vertex_indices& vertices, Edge_with_dir_indices& edges_with_direction) const
{
	auto halfedge = halfedge_circ();
	vertices[0] = halfedge->vertex_index();
	edges_with_direction[0] = {edge(**halfedge), is_first_halfedge(**halfedge)};
	vertices[1] = (++halfedge)->vertex_index();
	edges_with_direction[1] = {edge(**halfedge), is_first_halfedge(**halfedge)};
	vertices[2] = (++halfedge)->vertex_index();
	edges_with_direction[2] = {edge(**halfedge), is_first_halfedge(**halfedge)};

	assert(++halfedge == halfedge_circ());
}

void V::get_indices(
	Halfedge_index first, Vertex_indices& vertices, Halfedge_indices& halfedges) const
{
	assert(mesh_.face_index(first) == index_);

	auto halfedge = Halfedge_circ{mesh_, first};
	vertices[0] = halfedge->vertex_index();
	halfedges[0] = **halfedge;
	vertices[1] = (++halfedge)->vertex_index();
	halfedges[1] = **halfedge;
	vertices[2] = (++halfedge)->vertex_index();
	halfedges[2] = **halfedge;

	assert(**(++halfedge) == first);
}
} // namespace esf
