#pragma once
#include <esf/mesh/mesh2.hpp>
#include <esf/types.hpp>

#include <unordered_map>

namespace esf
{
// Creates a new mesh from the given mesh retaining the faces specified
// by the given predicate; the relative order of faces is preserved,
// the relative order of other mesh elements is not specified
template<class Unary_predicate>
Mesh2 mesh_filter_by_faces(const Mesh2& mesh, Unary_predicate pred)
{
	Mesh2 filtered_mesh;

	std::unordered_map<Vertex_index, Vertex_index> vertex_map;
	vertex_map.reserve(*mesh.n_vertices());

	for (auto& face : mesh.faces())
		if (pred(face))
		{
			Mesh2::Face_view::Vertex_indices indices;
			face.get_indices(indices);

			for (auto& index : indices)
				if (const auto it = vertex_map.find(index); it == vertex_map.end())
				{
					const auto new_index = filtered_mesh.add_vertex(mesh.vertex(index));
					vertex_map.insert({index, new_index});
					index = new_index;
				}
				else
					index = it->second;

			filtered_mesh.add_cell(indices);
		}

	filtered_mesh.shrink();
	return filtered_mesh;
}
} // namespace esf
