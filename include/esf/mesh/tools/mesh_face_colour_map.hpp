#pragma once
#include <esf/mesh/algorithm/colour_cells.hpp>
#include <esf/mesh/mesh2.hpp>

#include <esu/iterator.hpp>
#include <esu/tuple.hpp>

#include <cassert>
#include <unordered_map>
#include <utility>

namespace esf
{
class Mesh_face_colour_map
{
public:
	using Colour = unsigned int;

public:
	Mesh_face_colour_map(const Mesh2& mesh)
	{
		std::vector<Colour> colours;
		n_colours_ = colour_cells(mesh, colours);

		map_.reserve(*mesh.n_faces());
		for (Face_index face{0}; face < mesh.n_faces(); ++face)
			map_.emplace(colours[*face], face);
	}

	Colour n_colours() const
	{
		assert(!map_.empty());
		return n_colours_;
	}

	auto cells_with_colour(Colour colour) const
	{
		const auto range = map_.equal_range(colour);

		auto begin = esu::Transform_iterator{range.first, esu::Select_second{}};
		auto end = esu::Transform_iterator{range.second, esu::Select_second{}};
		return esu::Iterable{begin, end};
	}

private:
	Colour n_colours_ = 0;
	std::unordered_multimap<Colour, Face_index> map_;
};
} // namespace esf
