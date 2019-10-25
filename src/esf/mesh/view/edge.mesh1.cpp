#include <esf/geometry/point1.hpp>
#include <esf/index.hpp>
#include <esf/mesh/mesh1.hpp>

#include <cassert>
#include <cstddef>
#include <tuple>

namespace esf
{
bool Element_view<Edge_tag, Mesh1>::is_boundary() const
{
	return mesh_.is_boundary(**this);
}

const Point1& Element_view<Edge_tag, Mesh1>::vertex(std::size_t index) const
{
	return mesh_.vertex(vertex_index(index));
}

Vertex_index Element_view<Edge_tag, Mesh1>::vertex_index(std::size_t index) const
{
	assert(index < 2);
	if (index == 0)
		return mesh_.vertex_indices(index_).first;
	else
		return mesh_.vertex_indices(index_).second;
}

void Element_view<Edge_tag, Mesh1>::get_indices(Vertex_indices& vertices) const
{
	std::tie(vertices[0], vertices[1]) = mesh_.vertex_indices(index_);
}
} // namespace esf
