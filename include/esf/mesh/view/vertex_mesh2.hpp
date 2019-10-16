#pragma once
#include <esf/geometry/point2.hpp>
#include <esf/mesh/view/base.hpp>
#include <esf/types.hpp>

#include <iosfwd>

namespace esf
{
template<>
class Element_view<Vertex_tag, Mesh2> : public internal::Element_view_base<Vertex_tag, Mesh2>
{
public:
	using Vertex_circ = Circulator<Vertex_tag, Vertex_out_circ_tag>;

	using Halfedge_in_circ = Circulator<Halfedge_tag, Vertex_in_circ_tag>;
	using Halfedge_out_circ = Circulator<Halfedge_tag, Vertex_out_circ_tag>;

	using Edge_circ = Circulator<Edge_tag, Vertex_out_circ_tag>;
	using Face_circ = Circulator<Face_tag, Vertex_out_circ_tag>;

public:
	using Element_view_base::Element_view_base;

	const Point2& vertex() const;

	Vertex_circ vertex_circ() const;
	Halfedge_in_circ halfedge_in_circ() const;
	Halfedge_out_circ halfedge_out_circ() const;
	Edge_circ edge_circ() const;
	Face_circ face_circ() const;

	Local_index n_edges() const;
	Local_index n_faces() const;

	// Outputs human readable information about the view
	void print(std::ostream&) const;
};

// Outputs human readable information about the view
inline std::ostream& operator<<(std::ostream& os, const Element_view<Vertex_tag, Mesh2>& view)
{
	view.print(os);
	return os;
}
} // namespace esf
