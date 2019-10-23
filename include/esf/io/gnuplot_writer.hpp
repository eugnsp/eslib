#pragma once
#include <esf/element/lagrange.hpp>
#include <esf/io/text_file_writer.hpp>
#include <esf/solution_view.hpp>
#include <esf/type_traits.hpp>

#include <cassert>
#include <cstddef>
#include <fstream>
#include <string>

namespace esf
{
namespace internal
{
class Gnuplot_writer
{
public:
	Gnuplot_writer(const std::string& file_name) : writer_(file_name)
	{}

	template<class System, std::size_t var_idx, typename Value>
	void write(const Solution_view<System, var_idx, Value>& solution_view)
	{
		using Element = typename Var_type<System, var_idx>::Element;
		const auto scale = solution_view.mesh().output_scale();

		for (auto& face : solution_view.mesh().faces())
		{
			const auto values = solution_view[face];
			const auto vertices = face.vertices();
			for (const std::size_t i : {0, 1, 2, 2})
			{
				writer_.write(vertices[i].x() * scale, '\t', vertices[i].y() * scale);
				for (std::size_t dim = 0; dim < values.rows(); ++dim)
					if constexpr (Element::order > 0)
						writer_.write('\t', values(dim, i));
					else
						writer_.write('\t', values(dim, 0));
				writer_.write('\n');
				if (i == 1)
					writer_.write('\n');
			}
			writer_.write("\n\n");
		}
	}

private:
	internal::Text_file_writer writer_;
};
} // namespace internal

template<class System, std::size_t var_idx, typename Value>
void write_gnuplot(const std::string& file_name,
				   Solution_view<System, var_idx, Value> solution_view)
{
	internal::Gnuplot_writer writer(file_name);
	writer.write(solution_view);
}
} // namespace esf
