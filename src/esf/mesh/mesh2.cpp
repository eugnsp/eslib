#include <esf/geometry.hpp>
#include <esf/mesh/mesh2.hpp>

#include <esu/error.hpp>
#include <esu/string.hpp>

#include <algorithm>
#include <cassert>
#include <ostream>

namespace esf
{
// Returns the smallest rectangle that contains the mesh
Rect Mesh2::bounding_rect() const
{
	assert(!is_empty());

	const auto first = boundary_vertex_circ();

	esf::Point2 bottom_left = first->vertex();
	esf::Point2 top_right = first->vertex();

	auto circ = first;
	do
	{
		auto& v = circ->vertex();

		bottom_left.x() = std::min(bottom_left.x(), v.x());
		bottom_left.y() = std::min(bottom_left.y(), v.y());

		top_right.x() = std::max(top_right.x(), v.x());
		top_right.y() = std::max(top_right.y(), v.y());

		++circ;
	} while (circ != first);

	return {bottom_left, top_right};
}

// Performs some basic checks of mesh data structure consistency
esu::Error Mesh2::check() const
{
	auto err = Halfedge_structure::check();
	if (err)
		return err;

	// Check for counter-clockwise order of vertices
	for (auto& face : this->faces())
		if (area(face) < 0)
			err.append_ln("face #", **face, " has incorrect vertex order");

	return err;
}

// Outputs human readable information about the mesh
void Mesh2::print(std::ostream& os) const
{
	os << type_string() << '\n'
	   << "Vertices: " << *n_vertices() << '\n'
	   << "Edges:    " << *n_edges() << '\n'
	   << "Faces:    " << *n_faces() << '\n'
	   << "Memory:   " << esu::size_string(memory_size()) << std::endl;
}
} // namespace esf
