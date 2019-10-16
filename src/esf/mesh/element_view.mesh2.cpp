#include <esf/mesh/mesh2.hpp>
#include <esf/types.hpp>

#include <ostream>

namespace esf
{
// Outputs human readable information about the view
void Element_view<Vertex_tag, Mesh2>::print(std::ostream& os) const
{
	os << "Vertex #" << **(*this) << ": " << vertex() << std::endl;
}

// Outputs human readable information about the view
void Element_view<Halfedge_tag, Mesh2>::print(std::ostream& os) const
{
	os << "Helfedge #" << **(*this) << ": " << *vertex_index(0) << ' ' << vertex(0) << " -> "
	   << *vertex_index(1) << ' ' << vertex(1) << std::endl;
}

// Outputs human readable information about the view
void Element_view<Edge_tag, Mesh2>::print(std::ostream& os) const
{
	os << "Helfedge #" << **(*this) << ": " << *vertex_index(0) << ' ' << vertex(0) << " - "
	   << *vertex_index(1) << ' ' << vertex(1) << std::endl;
}

// Outputs human readable information about the view
void Element_view<Face_tag, Mesh2>::print(std::ostream& os) const
{
	Vertex_indices indices;
	get_indices(indices);

	os << "Face #" << **(*this) << ": " << *indices[0] << ", " << *indices[1] << ", " << *indices[2]
	   << "\n";

	auto circ = halfedge_circ();
	os << "  " << *circ;
	os << "  " << *(++circ);
	os << "  " << *(++circ);
}
} // namespace esf
