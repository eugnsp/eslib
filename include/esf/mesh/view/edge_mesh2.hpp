#pragma once
#include <esf/geometry/point2.hpp>
#include <esf/index.hpp>
#include <esf/mesh/view/base.hpp>
#include <esf/tags.hpp>

namespace esf
{
template<>
class Element_view<Edge_tag, Mesh2> : public internal::Element_view_base<Edge_tag, Mesh2>
{
public:
	using Geometry_tag = Segment2_tag;

public:
	using Element_view_base::Element_view_base;
	using Element_view_base::set_index;

	Edge_index operator*() const
	{
		return edge(this->index_);
	}

	void set_index(Edge_index index)
	{
		set_index(first_halfedge(index));
	}

	bool is_boundary() const;

	const Point2& vertex(std::size_t) const;
	Vertex_index vertex_index(std::size_t) const;

	// Outputs human readable information about the view
	void print(std::ostream&) const;
};

// Outputs human readable information about the view
inline std::ostream& operator<<(std::ostream& os, const Element_view<Edge_tag, Mesh2>& view)
{
	view.print(os);
	return os;
}
} // namespace esf
