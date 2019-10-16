// #pragma once
// #include <esf/mesh/tools/mesh_face_colour_map.hpp>
// #include <esf/types.hpp>

// #if _MSC_VER
// 	#define NOMINMAX
// #endif
// #include <tbb/tbb.h>

// #include <cassert>
// #include <memory>
// #include <utility>
// #include <vector>

// namespace esf
// {
// class Tbb_assembler
// {
// public:
// 	void init(std::shared_ptr<Mesh_face_colour_map> colour_map)
// 	{
// 		colour_map_ = std::move(colour_map);
// 	}

// 	template<class System, class Solver>
// 	void assemble(const System& system, Solver& solver)
// 	{
// 		assert(colour_map_);

// 		std::vector<Face_index> cells;

// 		const auto n_colours = colour_map_->n_colours();
// 		for (Mesh_face_colour_map::Colour_index colour = 0; colour < n_colours; ++colour)
// 		{
// 			colour_map_->get_cells_with_colour(colour, cells);

// 			tbb::parallel_for(
// 				tbb::blocked_range{cells.begin(), cells.end()}, [&solver](const auto& range) {
// 					for (auto cell = range.begin(); cell != range.end(); ++cell)
// 						solver.assemble_on_cell(solver.mesh().view(*cell));
// 					// 					auto cell_view =
// 					// solver.mesh().view(Face_index{}); 					for (auto
// 					// cell = range.begin(); cell != range.end(); ++cell)
// 					// 					{
// 					// 						cell_view.set_at(cell);
// 					// 						solver.assemble_on_cell(cell_view);
// 					// 					}
// 				});
// 		}
// 	}

// private:
// 	std::shared_ptr<Mesh_face_colour_map> colour_map_;
// };
// } // namespace esf
