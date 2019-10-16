#pragma once
#include <esf/geometry/point1.hpp>
#include <esf/mesh/view/base.hpp>
#include <esf/types.hpp>

namespace esf
{
template<>
class Element_view<Edge_tag, Mesh1> : public internal::Element_view_base<Edge_tag, Mesh1>
{
public:
	using Geometry_tag = Segment1_tag;

	using Vertex_indices = std::array<Vertex_index, 2>;

public:
	using Element_view_base::Element_view_base;

	bool is_boundary() const;

	const esf::Point1& vertex(Local_index) const;
	Vertex_index vertex_index(Local_index) const;

	void get_indices(Vertex_indices&) const;

	// Outputs human readable information about the view
	void print(std::ostream&) const;
};

// Outputs human readable information about the view
inline std::ostream& operator<<(std::ostream& os, const Element_view<Edge_tag, Mesh1>& view)
{
	view.print(os);
	return os;
}
} // namespace esf
