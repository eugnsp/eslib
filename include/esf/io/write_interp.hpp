#pragma once
#include <esf/geometry.hpp>
#include <esf/io/text_file_writer.hpp>
#include <esf/solution_view.hpp>

#include <cassert>
#include <cstddef>
#include <string>

namespace esf
{
// Writes the solution into a text file as a scattered data set in the format:
// <x> <y> <solution at (x, y)>
// where points <x> and <y> are obtained by fine-meshing faces such that the
// distance between any two neighbouring points is not greater than `dist`
template<class System, std::size_t var, typename Value>
void write_interp(
	const std::string& file_name, Solution_view<System, var, Value> solution_view, double dist)
{
	assert(dist > 0);

	internal::Text_file_writer writer(file_name);

	const auto& mesh = solution_view.mesh();
	for (auto& face : mesh.faces())
	{
		auto circ = face.vertex_circ();
		const auto v1 = circ->vertex();
		const auto v2 = (++circ)->vertex();
		const auto v3 = (++circ)->vertex();
		assert(++circ == face.vertex_circ());

		const auto n = static_cast<std::size_t>(std::ceil(diam(face) / dist));
		for (std::size_t i = 0; i <= n; ++i)
			for (std::size_t j = 0; j <= n - i; ++j)
			{
				const auto b1 = static_cast<double>(i) / n;
				const auto b2 = static_cast<double>(j) / n;
				const auto b3 = 1 - b1 - b2;
				esf::Point2 pt = mesh.output_scale() * (b1 * v1 + b2 * v2 + b3 * v3);
				writer.write_ln(pt.x(), '\t', pt.y(), '\t', solution_view.at(pt, face));
			}
	}
}
} // namespace esf
