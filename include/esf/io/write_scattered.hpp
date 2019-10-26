#pragma once
#include <esf/geometry.hpp>
#include <esf/io/text_file_writer.hpp>
#include <esf/solution_view.hpp>
#include <esf/type_traits.hpp>

#include <cassert>
#include <cstddef>
#include <string>

namespace esf
{
// Writes the solution into a text file as a scattered data set in the format:
// <x> <y> <solution at (x, y)>
template<class System, std::size_t var_idx, typename Value>
void write_scattered(const std::string& file_name,
                     Solution_view<System, var_idx, Value> solution_view)
{
	internal::Text_file_writer writer(file_name);

	using Element = typename Var_type<System, var_idx>::Element;
	const auto& mesh = solution_view.mesh();

	// TODO
	if constexpr (Element::has_face_dofs)
	{
		for (auto& face : mesh.faces())
		{
			const auto c = center(face);
			writer.write_ln(c.x(), '\t', c.y(), '\t', solution_view[*face]);
		}
	}
}
} // namespace esf
