#pragma once
#include <esf/mesh/mesh2.hpp>

#include <algorithm>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace esf
{
// Colours faces of the mesh using simple greedy algorithm such that
// no two faces having at least one common vertex have the same colour
template<class Colours>
auto colour_cells(const Mesh2& mesh, Colours& colours)
{
	using T = typename Colours::value_type;
	static_assert(std::is_unsigned_v<T>);
	constexpr auto invalid_colour = static_cast<T>(-1);

	colours.assign(*mesh.n_faces(), invalid_colour);

	T n_colours = 0;
	std::vector<T> used_colours;
	for (auto& face : mesh.faces())
	{
		used_colours.clear();

		// TODO : optimize
		const auto add_to_used = [&face, &mesh, &used_colours, &colours](Halfedge_index edge) {
			auto f = mesh.face_index(edge);
			if (is_valid(f) && f < *face)
				used_colours.push_back(colours[*f]);
		};

		auto halfedge = face.halfedge();
		mesh.for_each(halfedge, add_to_used, Vertex_out_circ_tag{});

		halfedge = mesh.next(halfedge, Face_circ_tag{});
		mesh.for_each(halfedge, add_to_used, Vertex_out_circ_tag{});

		halfedge = mesh.next(halfedge, Face_circ_tag{});
		mesh.for_each(halfedge, add_to_used, Vertex_out_circ_tag{});

		unsigned char face_colour = 0;

		std::sort(used_colours.begin(), used_colours.end());
		for (auto color : used_colours)
			if (face_colour == color)
				++face_colour;
			else if (face_colour < color)
				break;

		if (face_colour == invalid_colour)
			throw std::runtime_error("Mesh colouring error (too many colours needed)");

		colours[**face] = face_colour;
		if (n_colours <= face_colour)
			n_colours = face_colour + 1;
	}

	// 	for (auto& face : mesh.faces())
	// 	{
	// 		used_colours.clear();
	//
	// 		// TODO : optimize
	// 		const auto add_to_used = [face, &mesh, &used_colours, &colours](Halfedge_index edge)
	// 		{
	// 			auto f = mesh.face_index(edge);
	// 			if (is_valid(f) && f < face)
	// 				used_colours.push_back(colours[*f]);
	// 		};
	//
	// 		auto halfedge = mesh.halfedge_from_face(face);
	// 		mesh.for_each(halfedge, add_to_used, Vertex_out_circ_tag{});
	//
	// 		halfedge = mesh.next(halfedge, Face_circ_tag{});
	// 		mesh.for_each(halfedge, add_to_used, Vertex_out_circ_tag{});
	//
	// 		halfedge = mesh.next(halfedge, Face_circ_tag{});
	// 		mesh.for_each(halfedge, add_to_used, Vertex_out_circ_tag{});
	//
	// 		unsigned char face_colour = 0;
	//
	// 		std::sort(used_colours.begin(), used_colours.end());
	// 		for (auto color : used_colours)
	// 			if (face_colour == color)
	// 				++face_colour;
	// 			else if (face_colour < color)
	// 				break;
	//
	// 		if (face_colour == invalid_colour)
	// 			throw std::runtime_error("Mesh colouring error (too many colours needed)");
	//
	// 		colours[*face] = face_colour;
	// 		if (n_colours <= face_colour)
	// 			n_colours = face_colour + 1;
	// 	}

	return n_colours;
}
} // namespace esf
