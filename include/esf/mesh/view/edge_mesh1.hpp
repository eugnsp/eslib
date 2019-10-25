#pragma once
#include <esf/geometry/point1.hpp>
#include <esf/index.hpp>
#include <esf/mesh/view/base.hpp>
#include <esf/tags.hpp>

#include <array>
#include <cstddef>

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

	const Point1& vertex(std::size_t) const;
	Vertex_index vertex_index(std::size_t) const;

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
