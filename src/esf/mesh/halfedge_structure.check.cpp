#include <esf/geometry.hpp>
#include <esf/mesh/halfedge_structure.hpp>
#include <esf/types.hpp>

#include <esu/error.hpp>

#include <algorithm>
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
esu::Error Halfedge_structure::check() const
{
	esu::Error err;

	if (*n_vertices() == 0)
		err.append_ln("The mesh contains no vertices");

	if (*n_edges() == 0)
		err.append_ln("The mesh contains no edges");

	if (*n_cells() == 0)
		err.append_ln("The mesh contains no faces");

	if (err)
		return err;

	// Check vertices
	for (Index i = 0; i < *n_vertices(); ++i)
		if (vertices_[i].halfedge >= n_halfedges())
			err.append_ln("The vertex #", i, " has bad halfedge index (",
				index_string(vertices_[i].halfedge), ')');

	if (err)
		return err;

	// Check edges
	for (Index i = 0; i < *n_halfedges(); ++i)
	{
		auto& halfedge = halfedges_[i];

		if (halfedge.vertex >= n_vertices())
			err.append_ln(
				"The halfedge #", i, " has bad vertex index (", index_string(halfedge.vertex), ')');

		if (halfedge.next >= n_halfedges())
			err.append_ln("The halfedge #", i, " has bad next halfedge index (",
				index_string(halfedge.next), ')');

		// halfedge.face is invalid for outter halfedges
		if (is_valid(halfedge.face) && halfedge.face >= n_cells())
			err.append_ln(
				"The halfedge #", i, " has bad cell index (", index_string(halfedge.face), ')');
	}

	if (err)
		return err;

	// Check faces
	for (Index i = 0; i < *n_cells(); ++i)
		if (faces_[i].halfedge >= n_halfedges())
			err.append_ln("The face #", i, " has bad halfedge index (",
				index_string(faces_[i].halfedge), ')');

	if (err)
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
					err.append_ln("The halfedges of the face #", i, " do not form a cycle");
					return true;
				}

				seen_indices.insert(*edge);
				return false;
			},
			Face_circ_tag{});
	}

	if (err)
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
					err.append_ln("The halfedges of the vertex #", i, " do not form a cycle");
					return true;
				}

				seen_indices.insert(edge);
				return false;
			},
			Vertex_out_circ_tag{});
	}

	if (err)
		return err;

	// Check for duplicated vertices
	using Vertices = std::vector<std::pair<Index, esf::Point2>>;
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

		err.append_ln(
			"Vertices #", pos->first, " and #", (pos + 1)->first, " are identical, ", pos->second);
	}

	// Check for unused nodes
	std::vector<bool> seen_nodes(*n_vertices(), false);
	std::for_each(halfedges_.begin(), halfedges_.end(),
		[&seen_nodes](auto& edge) { seen_nodes[*edge.vertex] = true; });

	for (Index i = 0; i < *n_vertices(); ++i)
		if (!seen_nodes[i])
			err.append_ln("Vertex #", i, " is unused, ", vertices_[i].point);

	return err;
}
} // namespace esf::internal
