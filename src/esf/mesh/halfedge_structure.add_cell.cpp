#include <esf/mesh/halfedge_structure.hpp>
#include <esf/types.hpp>

#include <algorithm>
#include <array>
#include <cassert>
#include <utility>
#include <vector>

namespace esf::internal
{
Face_index Halfedge_structure::add_cell(const std::array<Vertex_index, 3>& vertices)
{
	assert(std::all_of(
		vertices.begin(), vertices.end(), [this](auto vi) { return vi < n_vertices(); }));

	// The code below is borrowed from OpenMesh
	struct Edges
	{
		Halfedge_index index;
		bool found;
		bool adjust = false;
	};

	std::vector<Edges> edges(3);
	std::vector<std::pair<Halfedge_index, Halfedge_index>> next_edges;
	next_edges.reserve(6 * 3);

	for (std::size_t i = 0; i < 3; ++i)
	{
		const auto ii = (i + 1) % 3;
		assert(is_boundary(vertices[i]));

		edges[i].index = find_halfedge(vertices[i], vertices[ii]);
		edges[i].found = is_valid(edges[i].index);

		assert(!edges[i].found || is_boundary(edges[i].index));
	}

	for (std::size_t i = 0; i < 3; ++i)
	{
		const auto ii = (i + 1) % 3;
		if (edges[i].found && edges[ii].found)
		{
			const auto prevInner = edges[i].index;
			const auto nextInner = edges[ii].index;

			if (next(prevInner, Face_circ_tag{}) != nextInner)
			{
				const auto prevOuter = twin(nextInner);
				// TODO : check
				// const auto nextOuter = twin(prevInner);
				auto prevBoundary = prevOuter;

				do
				{
					prevBoundary = twin(next(prevBoundary, Face_circ_tag{}));
				} while (!is_boundary(prevBoundary));

				const auto nextBoundary = next(prevBoundary, Face_circ_tag{});
				assert(prevBoundary != prevInner);
				assert(is_boundary(nextBoundary));

				const auto patchBegin = next(prevInner, Face_circ_tag{});
				const auto patchEnd = prev(nextInner, Face_circ_tag{});

				assert(is_valid(patchBegin));
				assert(is_valid(patchEnd));

				next_edges.push_back({prevBoundary, patchBegin});
				next_edges.push_back({patchEnd, nextBoundary});
				next_edges.push_back({prevInner, nextInner});
			}
		}
	}

	// Create missing edges
	for (std::size_t i = 0; i < 3; ++i)
		if (!edges[i].found)
		{
			const auto ii = (i + 1) % 3;
			edges[i].index = insert_halfedges_raw(vertices[i], vertices[ii]);
		}

	const auto cell = add_face_raw(edges.back().index);

	// Adjust the data structure
	for (std::size_t i = 0; i < 3; ++i)
	{
		const auto ii = (i + 1) % 3;
		auto& vertex = vertices_[*vertices[ii]];

		const auto prevInner = edges[i].index;
		const auto nextInner = edges[ii].index;

		if (!edges[i].found || !edges[ii].found)
		{
			const auto prevOuter = twin(nextInner);
			const auto nextOuter = twin(prevInner);

			// Previous is new, next is old
			if (!edges[i].found && edges[ii].found)
			{
				const auto prevBoundary = prev(nextInner, Face_circ_tag{});
				assert(is_valid(prevBoundary));

				next_edges.push_back({prevBoundary, nextOuter});
				vertex.halfedge = nextOuter;
			}
			// Previous is old, next is new
			else if (edges[i].found && !edges[ii].found)
			{
				const auto nextBoundary = next(prevInner, Face_circ_tag{});
				assert(is_valid(nextBoundary));

				next_edges.push_back({prevOuter, nextBoundary});
				vertex.halfedge = nextBoundary;
			}
			// Both edges are new
			else
			{
				if (!is_valid(vertex.halfedge))
				{
					vertex.halfedge = nextOuter;
					next_edges.push_back({prevOuter, nextOuter});
				}
				else
				{
					const auto nextBoundary = vertex.halfedge;
					const auto prevBoundary = prev(nextBoundary, Face_circ_tag{});
					assert(is_valid(prevBoundary));

					next_edges.push_back({prevBoundary, nextOuter});
					next_edges.push_back({prevOuter, nextBoundary});
				}
			}

			next_edges.push_back({prevInner, nextInner});
		}
		else
			edges[ii].adjust = (vertex.halfedge == nextInner);

		halfedges_[*prevInner].face = cell;
	}

	for (const auto& e : next_edges)
		halfedges_[*e.first].next = e.second;

	for (std::size_t i = 0; i < 3; ++i)
		if (edges[i].adjust)
			adjust_outgoing_halfedge(vertices[i]);

	return cell;
}
} // namespace esf::internal
