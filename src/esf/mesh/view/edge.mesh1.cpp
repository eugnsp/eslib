#include <esf/geometry.hpp>
#include <esf/mesh/mesh1.hpp>
#include <esf/types.hpp>

#include <cassert>
#include <tuple>

namespace esf
{
using V = Element_view<Edge_tag, Mesh1>;

bool V::is_boundary() const
{
	return mesh_.is_boundary(**this);
}

const esf::Point1& V::vertex(esf::Local_index index) const
{
	return mesh_.vertex(vertex_index(index));
}

esf::Vertex_index V::vertex_index(esf::Local_index index) const
{
	assert(index < 2);
	if (index == 0)
		return mesh_.vertex_indices(index_).first;
	else
		return mesh_.vertex_indices(index_).second;
}

void V::get_indices(Vertex_indices& vertices) const
{
	std::tie(vertices[0], vertices[1]) = mesh_.vertex_indices(index_);
}
} // namespace esf
