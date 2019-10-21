#include <esf/geometry.hpp>
#include <esf/mesh/mesh2.hpp>

#include <esu/string.hpp>

#include <algorithm>
#include <cassert>
#include <optional>
#include <ostream>
#include <string>

namespace esf
{
// Returns the smallest rectangle that contains the mesh
Rect Mesh2::bounding_rect() const
{
	assert(!is_empty());

	const auto first = boundary_vertex_circ();

	double x_min, x_max;
	double y_min, y_max;
	x_min = x_max = first->vertex().x();
	y_min = y_max = first->vertex().y();

	auto circ = first;
	do
	{
		auto& v = circ->vertex();

		x_min = std::min(x_min, v.x());
		y_min = std::min(y_min, v.y());
		x_max = std::max(x_max, v.x());
		y_max = std::max(y_max, v.y());
	} while (++circ != first);

	return {Point2{x_min, y_min}, Point2{x_max, y_max}};
}

// Performs some basic checks of mesh data structure consistency
std::optional<std::string> Mesh2::check() const
{
	if (const auto err = Halfedge_structure::check(); err)
		return err;

	// Check for counter-clockwise order of vertices
	std::string err_str;
	for (auto& face : this->faces())
		if (esf::area(face) < 0)
			err_str += "Face #" + std::to_string(**face) + " has incorrect vertex order\n";

	if (!err_str.empty())
		return err_str;
	else
		return {};
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
