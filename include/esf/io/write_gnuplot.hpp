#pragma once
#include <esf/io/gnuplot_writer2.hpp>
#include <esf/solution_view.hpp>

#include <esl/dense.hpp>
#include <esu/type_traits.hpp>

#include <cstddef>
#include <string>

namespace esf
{
template<class System, std::size_t var, typename Value>
void write_gnuplot(const std::string& file_name, Solution_view<System, var, Value> solution_view)
{
	using Var = typename System::template Var<var>;
	using Element = typename Var::Element;

	Gnuplot_writer2 writer(file_name, solution_view.mesh());

	// TODO
	if constexpr (Element::has_vertex_dofs)
	{
		esl::Vector_x<Value> phi(*solution_view.mesh().n_vertices());

		for (esf::Vertex_index vertex{0}; vertex < solution_view.mesh().n_vertices(); ++vertex)
			phi[*vertex] = solution_view[vertex];
		writer.write_vertex_field(phi);
	}
	else if constexpr (Element::has_face_dofs)
	{
		esl::Vector_x<Value> phi(*solution_view.mesh().n_faces());

		for (esf::Face_index face{0}; face < solution_view.mesh().n_faces(); ++face)
			phi[*face] = solution_view[face];
		writer.write_face_field(phi);
	}
}
} // namespace esf
