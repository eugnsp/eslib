#include <esf/mesh/mesh1.hpp>
#include <esf/types.hpp>

namespace esf
{
using V = Element_view<Vertex_tag, Mesh1>;

Point1 V::vertex() const
{
	return mesh_.vertex(index_);
}
} // namespace esf
