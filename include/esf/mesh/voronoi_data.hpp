#pragma once
#include <cassert>
#include <cstddef>
#include <esf/mesh/mesh2.hpp>
#include <es_geom/algorithm.hpp>
#include <esu/error.hpp>
#include <vector>

#include <iostream>

namespace esf
{
class Voronoi_data
{
public:
	Voronoi_data(const Mesh2& mesh) : mesh_(mesh)
	{}

	void precompute()
	{
		data_.resize(2 * mesh_.n_edges(), {});

		for (auto edge = mesh_.begin_edge(); edge != mesh_.end_edge(); ++edge)
			for (Local_index k = 0; k < 2; ++k)
			{
				const auto face = edge->face_view(k);
				if (!face.is_valid())
					continue;

				// TODO
				const double edge_length = length(*edge);
				const auto edge_midpoint = center(*edge);

				const auto cc = circumcentre(face);
				const auto side = (k == 0) ? Side::ON_THE_LEFT : Side::ON_THE_RIGHT;
				const auto sign = 1;
				throw 0; // (which_side(cc, *edge) == side) ? 1 : -1;
				const double interface_length = sign * distance(edge_midpoint, cc);

				data_[2 * **edge + k] = {interface_length, interface_length * edge_length / 4};
			}

		// TODO : check for negative lengths, set length to zero and correct cell area
		// (see MATLAB 'voronoi')

		//		check().throw_if_error();
	}

	double edge_length(Index edge) const
	{
		return data_[2 * edge].lenght + data_[2 * edge + 1].lenght;
	}

	double half_edge_length(Index half_edge) const
	{
		return data_[half_edge].lenght;
	}

	double cell_volume(Index edge) const
	{
		return data_[2 * edge].volume + data_[2 * edge + 1].volume;
	}

	double half_cell_volume(Index half_edge) const
	{
		return data_[half_edge].volume;
	}

	es::util::Error check() const
	{
		es::util::Error err("Voronoi data check");
		for (auto edge = mesh_.begin_edge(); edge != mesh_.end_edge(); ++edge)
		{
			if (edge->is_boundary())
				continue;

			if (edge_length(**edge) < 0)
			{
				err << "Negative Voronoi edge length (" << edge_length(**edge) << ") for " << *edge
					<< '\n';
				err.set_error_flag();
			}
		}

		return err;
	}

	// Returns approximate total size of memory in bytes occupied by the container
	std::size_t memory_size() const
	{
		return data_.capacity() * sizeof(data_[0]);
	}

private:
	struct Data
	{
		double lenght = 0; // Voronoi half-edge length
		double volume = 0; // Voronoi half-cell volume
	};

private:
	std::vector<Data> data_;
	const Mesh2& mesh_;
};
} // namespace esf
