#pragma once
#include <esf/geometry/point1.hpp>
#include <esf/mesh/tags.hpp>
#include <esf/mesh/view/base.hpp>

#include <iosfwd>

namespace esf
{
template<>
class Element_view<Vertex_tag, Mesh1> : public internal::Element_view_base<Vertex_tag, Mesh1>
{
public:
	using Element_view_base::Element_view_base;

	Point1 vertex() const;

	// Outputs human readable information about the view
	void print(std::ostream&) const;
};

// Outputs human readable information about the view
inline std::ostream& operator<<(std::ostream& os, const Element_view<Vertex_tag, Mesh1>& view)
{
	view.print(os);
	return os;
}
} // namespace esf
