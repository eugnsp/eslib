#pragma once
#include <esf/types.hpp>

namespace esf::internal
{
// 1D linear element
//		vertex_dofs		number of internal dofs per vertex
//		edge_dofs		number of internal dofs per edge
template<Local_index vertex_dofs, Local_index edge_dofs>
class Linear_element
{
public:
	static constexpr Local_index dim = 1;

	// The number of internal vertex dofs
	static constexpr Local_index n_vertex_dofs = vertex_dofs;

	// The number of internal edge dofs
	static constexpr Local_index n_edge_dofs = edge_dofs;
	static constexpr Local_index n_cell_dofs = edge_dofs;

	// The number of total edge dofs (vertex + edge dofs)
	static constexpr Local_index n_total_edge_dofs = 2 * vertex_dofs + edge_dofs;
	static constexpr Local_index n_total_cell_dofs = n_total_edge_dofs;

	// True if the element has internal vertex dofs
	static constexpr bool has_vertex_dofs = (n_vertex_dofs > 0);

	// True if the element has internal edge dofs
	static constexpr bool has_edge_dofs = (n_edge_dofs > 0);
	static constexpr bool has_cell_dofs = has_edge_dofs;

	// Returns the number of internal vertex dofs
	static constexpr Local_index n_dofs(Vertex_tag)
	{
		return n_vertex_dofs;
	}

	// Returns the number of internal edge dofs
	static constexpr Local_index n_dofs(Edge_tag)
	{
		return n_edge_dofs;
	}

	static constexpr Local_index n_dofs(Cell_tag)
	{
		return n_edge_dofs;
	}

	// Returns the number of total edge dofs (vertex + edge dofs)
	static constexpr Local_index n_total_dofs(Edge_tag)
	{
		return n_total_edge_dofs;
	}

	static constexpr Local_index n_total_dofs(Cell_tag)
	{
		return n_total_edge_dofs;
	}

	// Checks if the element has dofs of the given type
	template<class Tag>
	static constexpr bool has_dofs(Tag tag)
	{
		return n_dofs(tag) > 0;
	}
};
} // namespace esf::internal
