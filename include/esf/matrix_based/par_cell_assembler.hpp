#pragma once
#include <esf/mesh/tools/mesh_face_colour_map.hpp>

#include <esu/thread.hpp>

#include <cstddef>
#include <mutex>
#include <thread>
#include <vector>

namespace esf
{
class Par_cell_assembler
{
public:
	Par_cell_assembler(const esf::Mesh_face_colour_map& colour_map) : colour_map_(colour_map)
	{}

	template<class System, class Fn>
	void assemble(const System& system, Fn&& fn)
	{
		using Colour = esf::Mesh_face_colour_map::Colour;

		const Colour n_colours = colour_map_.n_colours();
		Colour colour = 0;
		auto range = colour_map_.cells_with_colour(colour);

		const auto n_workers = n_threads();
		esu::Barrier barrier(n_workers, [&, this] {
			if (++colour < n_colours)
				range = colour_map_.cells_with_colour(colour);
		});

		std::mutex range_mutex;
		const auto assemble_for_colour = [&] {
			while (colour < n_colours)
			{
				auto face = esf::Face_index::invalid;
				{
					std::scoped_lock lock{range_mutex};
					if (!range.empty())
						face = *range.begin()++;
				}

				if (is_valid(face))
					fn(system.mesh().view(face));
				else
					barrier.wait();
			}
		};

		std::vector<std::thread> workers;
		for (unsigned int i = 1; i < n_workers; ++i)
			workers.push_back(std::thread(assemble_for_colour));

		assemble_for_colour();

		for (auto& worker : workers)
			worker.join();
	}

private:
	static std::size_t n_threads()
	{
		const auto n = std::thread::hardware_concurrency();
		return (n > 0) ? n : 1;
	}

private:
	const esf::Mesh_face_colour_map& colour_map_;
};
} // namespace esf
