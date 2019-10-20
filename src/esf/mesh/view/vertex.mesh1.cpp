#include <esf/geometry/point1.hpp>
#include <esf/mesh/mesh1.hpp>

namespace esf
{
Point1 Element_view<Vertex_tag, Mesh1>::vertex() const
{
	return mesh_.vertex(index_);
}
} // namespace esf
