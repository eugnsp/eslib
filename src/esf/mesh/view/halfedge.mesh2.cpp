#include <esf/geometry.hpp>
#include <esf/mesh/mesh2.hpp>
#include <esf/types.hpp>

#include <cassert>

namespace esf
{
using V = Element_view<Halfedge_tag, Mesh2>;

const esf::Point2& V::vertex() const
{
	return mesh_.vertex(vertex_index());
}

const esf::Point2& V::vertex(Local_index vertex) const
{
	return mesh_.vertex(vertex_index(vertex));
}

Vertex_index V::vertex_index() const
{
	return mesh_.vertex_index(index_);
}

Vertex_index V::vertex_index(Local_index vertex) const
{
	assert(vertex < 2);
	if (vertex == 0)
		return mesh_.vertex_index(index_);
	else
		return mesh_.vertex_index(twin(index_));
}

esf::Point2 V::vector() const
{
	return esf::Point2{vertex() - mesh_.vertex(mesh_.vertex_index(twin(index_)))};
}

Edge_index V::edge_index() const
{
	return edge(index_);
}

auto V::vertex_view() const -> Vertex_view
{
	return {mesh_, vertex_index()};
}

Face_index V::face_index() const
{
	return mesh_.face_index(index_);
}

Face_index V::twin_face_index() const
{
	return mesh_.face_index(twin(index_));
}

Face_index V::face_index(Local_index index) const
{
	assert(index < 2);
	if (index == 0)
		return face_index();
	else
		return twin_face_index();
}

auto V::face_view() const -> Face_view
{
	return {mesh_, face_index()};
}

auto V::twin_face_view() const -> Face_view
{
	return {mesh_, twin_face_index()};
}

auto V::face_view(Local_index index) const -> Face_view
{
	return {mesh_, face_index(index)};
}
} // namespace esf
