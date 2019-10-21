#pragma once
#include <esf/tags.hpp>
#include <esf/type_traits.hpp>

#include <cstddef>

namespace esf::internal
{
template<class Space_dim_,
		 std::size_t vertex_dofs_,
		 std::size_t edge_dofs_,
		 std::size_t face_dofs_ = 0>
class Element_base
{
	static_assert(is_dim2<Space_dim_> || face_dofs_ == 0);

public:
	using Space_dim = Space_dim_;

	// The number of vertex dofs
	static constexpr std::size_t vertex_dofs = vertex_dofs_;
	static constexpr bool has_vertex_dofs = vertex_dofs > 0;

	// The number of edge dofs
	static constexpr std::size_t edge_dofs = edge_dofs_;
	static constexpr bool has_edge_dofs = edge_dofs > 0;

	// The number of face dofs
	static constexpr std::size_t face_dofs = face_dofs_;
	static constexpr bool has_face_dofs = face_dofs > 0;

	// The number of cell dofs
	static constexpr std::size_t cell_dofs = is_dim1<Space_dim> ? edge_dofs : face_dofs;
	static constexpr bool has_cell_dofs = cell_dofs > 0;

	// The total number of edge dofs (vertex + edge dofs)
	static constexpr std::size_t total_edge_dofs = 2 * vertex_dofs + edge_dofs;

	// The total number of face dofs (vertex + edge + face dofs)
	static constexpr std::size_t total_face_dofs =
		is_dim1<Space_dim> ? 0 : 3 * vertex_dofs + 3 * edge_dofs + face_dofs;

	// The total number of cell dofs
	static constexpr std::size_t total_cell_dofs =
		is_dim1<Space_dim> ? total_edge_dofs : total_face_dofs;

	// Returns the number of vertex dofs
	static constexpr std::size_t dofs(Vertex_tag)
	{
		return vertex_dofs;
	}

	// Returns the number of edge dofs
	static constexpr std::size_t dofs(Edge_tag)
	{
		return edge_dofs;
	}

	// Returns the number of face dofs
	static constexpr std::size_t dofs(Face_tag)
	{
		return face_dofs;
	}

	// Returns the number of cell dofs
	static constexpr std::size_t dofs(Cell_tag)
	{
		return cell_dofs;
	}

	// Returns the number of total edge dofs (vertex + edge dofs)
	static constexpr std::size_t total_dofs(Edge_tag)
	{
		return total_edge_dofs;
	}

	// Returns the number of total edge dofs (vertex + edge + face dofs)
	static constexpr std::size_t total_dofs(Face_tag)
	{
		return total_face_dofs;
	}

	static constexpr std::size_t total_dofs(Cell_tag)
	{
		return total_cell_dofs;
	}

	// Checks if the element has dofs of the given type
	template<class Tag>
	static constexpr bool has_dofs(Tag tag)
	{
		return dofs(tag) > 0;
	}
};
} // namespace esf::internal
