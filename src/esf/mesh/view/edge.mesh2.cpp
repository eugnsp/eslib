#include <esf/geometry.hpp>
#include <esf/mesh/mesh2.hpp>
#include <esf/types.hpp>

#include <cassert>

namespace esf
{
using V = Element_view<Edge_tag, Mesh2>;

bool V::is_boundary() const
{
	return mesh_.is_boundary(**this);
}

const esf::Point2& V::vertex(Local_index index) const
{
	return mesh_.vertex(vertex_index(index));
}

Vertex_index V::vertex_index(Local_index index) const
{
	assert(index < 2);

	if (index == 0)
		return mesh_.vertex_index(index_);
	else
		return mesh_.vertex_index(twin(index_));
}
} // namespace esf
