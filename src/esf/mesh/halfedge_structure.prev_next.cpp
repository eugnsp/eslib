#include <esf/mesh/halfedge_structure.hpp>
#include <esf/types.hpp>

namespace esf::internal
{
auto Halfedge_structure::prev(Halfedge_index index, Vertex_in_circ_tag) const -> Halfedge_index
{
	return prev(twin(index), Face_circ_tag{});
}

auto Halfedge_structure::prev(Halfedge_index index, Vertex_out_circ_tag) const -> Halfedge_index
{
	return twin(prev(index, Face_circ_tag{}));
}

auto Halfedge_structure::prev(Halfedge_index index, Face_circ_tag) const -> Halfedge_index
{
	// TO DO : use loop?

	auto prev = index;
	auto curr = index;

	do
	{
		prev = curr;
		curr = next(prev, Face_circ_tag{});
	} while (curr != index);

	return prev;
}

auto Halfedge_structure::next(Halfedge_index index, Vertex_in_circ_tag) const -> Halfedge_index
{
	return twin(next(index, Face_circ_tag{}));
}

auto Halfedge_structure::next(Halfedge_index index, Vertex_out_circ_tag) const -> Halfedge_index
{
	return next(twin(index), Face_circ_tag{});
}

auto Halfedge_structure::next(Halfedge_index index, Face_circ_tag) const -> Halfedge_index
{
	return halfedges_[*index].next;
}
} // namespace esf::internal
