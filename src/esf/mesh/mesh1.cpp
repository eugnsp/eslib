#include <esf/geometry.hpp>
#include <esf/mesh/mesh1.hpp>

#include <algorithm>

namespace esf
{
esf::Vertex_index Mesh1::find_vertex(esf::Point1 point) const
{
	const auto pos = std::lower_bound(vertices_.begin(), vertices_.end(), point);
	if (pos != vertices_.end() && *pos == point)
		return static_cast<esf::Vertex_index>(pos - vertices_.begin());
	else
		return esf::Vertex_index::invalid;
}

bool Mesh1::is_boundary(esf::Vertex_index index) const
{
	return *index == 0 || index == (n_vertices() - 1);
}

bool Mesh1::is_boundary(esf::Edge_index index) const
{
	return *index == 0 || index == (n_edges() - 1);
}

std::pair<esf::Vertex_index, esf::Vertex_index> Mesh1::vertex_indices(
	esf::Edge_index edge) const
{
	const auto first_vertex = esf::Vertex_index{*edge};
	return {first_vertex, first_vertex + 1};
}
} // namespace esf
