#include <esf/geometry.hpp>
#include <esf/index.hpp>
#include <esf/mesh/mesh2.hpp>
#include <esf/tags.hpp>

#include <cassert>
#include <cstddef>

namespace esf
{
bool Element_view<Edge_tag, Mesh2>::is_boundary() const
{
	return mesh_.is_boundary(**this);
}

const Point2& Element_view<Edge_tag, Mesh2>::vertex(std::size_t index) const
{
	return mesh_.vertex(vertex_index(index));
}

Vertex_index Element_view<Edge_tag, Mesh2>::vertex_index(std::size_t index) const
{
	assert(index < 2);

	if (index == 0)
		return mesh_.vertex_index(index_);
	else
		return mesh_.vertex_index(twin(index_));
}
} // namespace esf
