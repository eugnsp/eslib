#include <esf/geometry.hpp>
#include <esf/mesh/halfedge_structure.hpp>
#include <esf/mesh/index.hpp>

#include <algorithm>
#include <optional>
#include <set>
#include <string>
#include <utility>

template<class Index>
std::string index_string(Index index)
{
	return is_valid(index) ? std::to_string(*index) : "<invalid index>";
}

namespace esf::internal
{
// Performs some basic checks of mesh data structure consistency
std::optional<std::string> Halfedge_structure::check() const
{
	std::string err;

	if (*n_vertices() == 0)
		err += "The mesh contains no vertices\n";

	if (*n_edges() == 0)
		err += "The mesh contains no edges\n";

	if (*n_cells() == 0)
		err += "The mesh contains no faces\n";

	if (!err.empty())
		return err;

	// Check vertices
	for (Index i = 0; i < *n_vertices(); ++i)
		if (vertices_[i].halfedge >= n_halfedges())
			err += "The vertex #" + std::to_string(i) + " has bad halfedge index " +
				   index_string(vertices_[i].halfedge) + '\n';

	if (!err.empty())
		return err;

	// Check edges
	for (Index i = 0; i < *n_halfedges(); ++i)
	{
		auto& halfedge = halfedges_[i];

		if (halfedge.vertex >= n_vertices())
			err += "The halfedge #" + std::to_string(i) + " has bad vertex index " +
				   index_string(halfedge.vertex) + '\n';

		if (halfedge.next >= n_halfedges())
			err += "The halfedge #" + std::to_string(i) + " has bad next halfedge index " +
				   index_string(halfedge.next) + '\n';

		// halfedge.face is invalid for outter halfedges
		if (is_valid(halfedge.face) && halfedge.face >= n_cells())
			err += "The halfedge #" + std::to_string(i) + " has bad cell index " +
				   index_string(halfedge.face) + '\n';
	}

	if (!err.empty())
		return err;

	// Check faces
	for (Index i = 0; i < *n_cells(); ++i)
		if (faces_[i].halfedge >= n_halfedges())
			err += "The face #" + std::to_string(i) + " has bad halfedge index " +
				   index_string(faces_[i].halfedge) + '\n';

	if (!err.empty())
		return err;

	// Check face-based cycles
	for (Index i = 0; i < *n_cells(); ++i)
	{
		std::set<Index> seen_indices;

		find_if(
			faces_[i].halfedge,
			[i, &seen_indices, &err](Halfedge_index edge) {
				if (seen_indices.count(*edge))
				{
					err += "The halfedges of the face #" + std::to_string(i) +
						   " do not form a cycle\n";
					return true;
				}

				seen_indices.insert(*edge);
				return false;
			},
			Face_circ_tag{});
	}

	if (!err.empty())
		return err;

	// Check vertex-based cycles
	for (Index i = 0; i < *n_vertices(); ++i)
	{
		std::set<Halfedge_index> seen_indices;

		find_if(
			vertices_[i].halfedge,
			[i, &seen_indices, &err](Halfedge_index edge) {
				if (seen_indices.count(edge))
				{
					err += "The halfedges of the vertex #" + std::to_string(i) +
						   " do not form a cycle";
					return true;
				}

				seen_indices.insert(edge);
				return false;
			},
			Vertex_out_circ_tag{});
	}

	if (!err.empty())
		return err;

	// Check for duplicated vertices
	using Vertices = std::vector<std::pair<Index, Point2>>;
	Vertices vertices;
	vertices.reserve(*n_vertices());

	for (Index i = 0; i < *n_vertices(); ++i)
		vertices.emplace_back(i, vertices_[i].point);

	std::sort(
		vertices.begin(), vertices.end(), [](auto& v1, auto& v2) { return v1.second < v2.second; });

	for (auto pos = vertices.begin(); pos != vertices.end(); ++pos)
	{
		const auto are_equal_points = [](auto& v1, auto& v2) { return v1.second == v2.second; };
		pos = std::adjacent_find(pos, vertices.end(), are_equal_points);
		if (pos == vertices.end())
			break;

		err += "Vertices #" + std::to_string(pos->first) + " and #" +
			   std::to_string((pos + 1)->first) + " are identical, " + pos->second.to_string() +
			   '\n';
	}

	// Check for unused nodes
	std::vector<bool> seen_nodes(*n_vertices(), false);
	std::for_each(halfedges_.begin(), halfedges_.end(),
		[&seen_nodes](auto& edge) { seen_nodes[*edge.vertex] = true; });

	for (Index i = 0; i < *n_vertices(); ++i)
		if (!seen_nodes[i])
			err += "Vertex #" + std::to_string(i) + " is unused, " +
				   vertices_[i].point.to_string() + '\n';

	if (!err.empty())
		return err;
	else
		return {};
}
} // namespace esf::internal
