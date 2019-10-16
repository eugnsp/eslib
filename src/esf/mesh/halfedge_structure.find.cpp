#include <esf/geometry.hpp>
#include <esf/mesh/halfedge_structure.hpp>
#include <esf/types.hpp>

#include <algorithm>

namespace esf::internal
{
auto Halfedge_structure::find_vertex(const esf::Point2& point) const -> Vertex_index
{
	const auto pos = std::find_if(vertices_.begin(), vertices_.end(),
		[&point](auto& vertex) { return vertex.point == point; });

	if (pos != vertices_.end())
		return Vertex_index{static_cast<Index>(pos - vertices_.begin())};
	else
		return Vertex_index::invalid;
}

auto Halfedge_structure::find_halfedge(Vertex_index from, Vertex_index to) const -> Halfedge_index
{
	const auto halfedge = vertices_[*to].halfedge;
	if (!is_valid(halfedge))
		return Halfedge_index::invalid;

	const auto invalid_or_required = [from, this](Halfedge_index edge) {
		// TODO : need !is_valid for construction?
		return !is_valid(edge) || halfedges_[*edge].vertex == from;
	};

	return find_if(twin(halfedge), invalid_or_required, Vertex_in_circ_tag{});
}
} // namespace esf::internal
