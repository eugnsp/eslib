#pragma once
#include <cassert>
#include <esf/mesh/mesh2.hpp>
#include <esf/types.hpp>
#include <esl/io/matfile_writer.hpp>
#include <string>

namespace esf
{
class Matlab_writer2
{
public:
	Matlab_writer2(const std::string& file_name, const Mesh2& mesh, double mesh_scale = 1) :
		mat_file_(file_name), mesh_(mesh)
	{
		write_mesh(mesh_scale);
	}

	template<class Field>
	void write_vertex_field(const std::string& var_name, const Field& field)
	{
		assert(field.size() == *mesh_.n_vertices());
		mat_file_.write(var_name, field);
	}

	template<class Field>
	void write_face_scalar_field(const std::string& var_name, const Field& field)
	{
		assert(field.size() == *mesh_.n_faces());

		esl::Vector_xd vertex_field(*mesh_.n_vertices(), 0), norm(*mesh_.n_vertices(), 0);
		for (auto& cell : mesh_.faces())
		{
			std::array<Vertex_index, 3> vertices;
			cell.get_indices(vertices);

			for (Local_index r = 0; r < 3; ++r)
			{
				vertex_field[*vertices[r]] += field[**cell];
				++norm[*vertices[r]];
			}
		}

		for (std::size_t i = 0; i < *mesh_.n_vertices(); ++i)
			vertex_field[i] /= norm[i];

		write_vertex_field(var_name, vertex_field);
	}

	// void writeDataOnCells(const std::string&, const la::Vector_xd&, double scale = 1);
	// void _writeDataOnEdges(const std::string&, const la::Vector_xd&, double scale = 1);

	template<typename T>
	void write_scalar(const std::string& var_name, T value)
	{
		mat_file_.write(var_name, value);
	}

private:
	void write_mesh(double mesh_scale)
	{
		// Vertices
		esl::Matrix_xd vertices(2, *mesh_.n_vertices());
		for (Vertex_index vertex{0}; vertex < mesh_.n_vertices(); ++vertex)
		{
			vertices(0, *vertex) = mesh_.vertex(vertex).x() / mesh_scale;
			vertices(1, *vertex) = mesh_.vertex(vertex).y() / mesh_scale;
		}

		mat_file_.write("vertices", vertices);

		// Faces
		esl::Matrix_x<Index> tri_faces(3, *mesh_.n_faces());

		std::size_t tri_face_index = 0;
		for (auto& face : mesh_.faces())
		{
			Mesh2::Face_view::Vertex_indices vertices;
			face.get_indices(vertices);

			if (vertices.size() == 3)
			{
				for (std::size_t i = 0; i < 3; ++i)
					tri_faces(i, tri_face_index) = *vertices[i] + 1;
				++tri_face_index;
			}
			else
				// Simple triangulation
				for (std::size_t r = 1; r < vertices.size() - 1; ++r)
				{
					tri_faces(0, tri_face_index) = *vertices[0] + 1;
					tri_faces(1, tri_face_index) = *vertices[r] + 1;
					tri_faces(2, tri_face_index) = *vertices[r + 1] + 1;
					++tri_face_index;
				}
		}

		mat_file_.write("faces", tri_faces);
	}

private:
	esl::Matfile_writer mat_file_;
	const Mesh2& mesh_;
};
} // namespace esf
