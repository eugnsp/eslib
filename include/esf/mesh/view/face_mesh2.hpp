#pragma once
#include <esf/mesh/view/base.hpp>
#include <esf/type_traits.hpp>
#include <esf/types.hpp>

#include <array>
#include <utility>

namespace esf
{
template<>
class Element_view<Face_tag, Mesh2> : public internal::Element_view_base<Face_tag, Mesh2>
{
public:
	using Geometry_tag = Triangle_tag;

	using Vertex_indices = std::array<Vertex_index, 3>;
	using Halfedge_indices = std::array<Halfedge_index, 3>;
	using Edge_indices = std::array<Edge_index, 3>;
	using Edge_with_dir_indices = std::array<std::pair<Edge_index, bool>, 3>;

public:
	using Vertex_circ = Circulator<Vertex_tag, Face_circ_tag>;
	using Halfedge_circ = Circulator<Halfedge_tag, Face_circ_tag>;
	using Edge_circ = Circulator<Edge_tag, Face_circ_tag>;
	using Face_circ = Circulator<Face_tag, Face_circ_tag>;

public:
	using Element_view_base::Element_view_base;

	Halfedge_index halfedge() const;

	Vertex_circ vertex_circ() const;
	Halfedge_circ halfedge_circ() const;
	Edge_circ edge_circ() const;
	Face_circ face_circ() const;

	void get_indices(Vertex_indices&) const;
	void get_indices(Vertex_indices&, Edge_indices&) const;
	void get_indices(Vertex_indices&, Halfedge_indices&) const;
	void get_indices(Vertex_indices&, Edge_with_dir_indices&) const;

	void get_indices(Halfedge_index, Vertex_indices&, Halfedge_indices&) const;

	// 	Vertex_index vertex_index(Local_index) const;
	// 	void vertex_indices(std::vector<Vertex_index>&) const;
	// 	void vertex_and_edge_indices(std::vector<Vertex_index>& vertices, std::vector<Edge_index>&
	// edges) const;
	//
	// 	// edges[i].second is true (false) if the corresponding edge view
	// 	// has this face on the left (right)
	// 	void vertex_and_edge_indices(std::vector<Vertex_index>& vertices,
	// 		std::vector<std::pair<Edge_index, bool>>& edges) const;
	//
	// #ifdef TRIANGULAR_CELLS_ONLY
	// 	void vertex_indices(Vertex_indices&) const;
	// 	void edge_indices(Edge_indices&) const;
	// 	void vertex_and_edge_indices(Vertex_indices& vertices, Edge_indices& edges) const;
	//
	// 	// edges[i].second is true (false) if the corresponding edge view
	// 	// has this face on the left (right)
	// 	void vertex_and_edge_indices(Vertex_indices& vertices, Directed_edge_indices& edges) const;
	// #endif
	//
	// 	/** Returns a vertex by the local index of the vertex in the cell */
	// 	const geom::Point& vertex(Local_index index) const;
	//
	// #ifdef TRIANGULAR_CELLS_ONLY
	// 	template<Local_index index>
	// 	const geom::Point& vertex() const
	// 	{
	// 		static_assert(index < 3, "Index out of bounds");
	// 		return vertex(index);
	// 	}
	// #endif

	// Outputs human readable information about the view
	void print(std::ostream&) const;
};

// Outputs human readable information about the view
inline std::ostream& operator<<(std::ostream& os, const Element_view<Face_tag, Mesh2>& view)
{
	view.print(os);
	return os;
}
} // namespace esf
