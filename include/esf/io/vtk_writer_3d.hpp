#pragma once
#include <cassert>
#include <esf/mesh/mesh2.hpp>
#include <esf/types.hpp>
#include <esu/time.hpp>
#include <fstream>
#include <stdexcept>
#include <string>
#include <vector>

namespace esf
{
class Vtk_writer_3d
{
public:
	Vtk_writer_3d(const std::string& file_name, const Mesh<2>& mesh,
		const std::vector<double>& grid_z, double mesh_scale = 1) :
		file_(file_name),
		mesh_(mesh), grid_z_(grid_z)
	{
		if (!file_)
			throw std::runtime_error("File '" + file_name + "' cannot be opened for writing");

		write_header();
		write_mesh(mesh_scale);
	}

	template<class Field>
	void write_vertex_scalar_field(const std::string& var_name, const Field& field)
	{
		//		assert(field.size() == mesh_.n_vertices());

		write_ln("POINT_DATA ", *mesh_.n_vertices() * grid_z_.size());
		write_scalar_field(var_name, field);
	}

	// 	template<class Field>
	// 	void write_vertex_vector_field(const std::string& var_name, const Field& field)
	// 	{
	// 		assert(field.rows() == 2 && field.cols() == mesh_.n_vertices());
	//
	// 		write_ln("POINT_DATA ", mesh_.n_vertices());
	// 		write_vector_field(var_name, field);
	// 	}
	//
	// 	template<class Field>
	// 	void write_faces_scalar_field(const std::string& var_name, const Field& field)
	// 	{
	// 		assert(field.size() == mesh_.n_faces());
	//
	// 		write_ln("CELL_DATA ", mesh_.n_faces());
	// 		write_scalar_field(var_name, field);
	// 	}
	//
	// 	template<class Field>
	// 	void write_faces_vector_data(const std::string& var_name, const Field& data)
	// 	{
	// 		assert(data.rows() == 2 && data.cols() == mesh_.n_faces());
	//
	// 		write_ln("CELL_DATA ", mesh_.n_faces());
	// 		write_vector_field(var_name, data);
	// 	}

private:
	template<typename... Ts>
	void write(const Ts&... args)
	{
		(file_ << ... << args);
		if (file_.bad())
			throw std::runtime_error("Error writing file");
	}

	template<typename... Ts>
	void write_ln(const Ts&... args)
	{
		(file_ << ... << args);
		file_ << '\n';
		if (file_.bad())
			throw std::runtime_error("Error writing file");
	}

	void write_header()
	{
		write_ln("# vtk DataFile Version 3.0");
		write_ln("VTK-file, created on: ", esu::date_time_string());
		write_ln("ASCII");
		write_ln();
	}

	void write_mesh(double mesh_scale)
	{
		constexpr unsigned int VTK_WEDGE = 13;

		write_ln("DATASET UNSTRUCTURED_GRID");
		write_ln("POINTS ", *mesh_.n_vertices() * grid_z_.size(), " float");

		// Vertices
		for (const auto& vertex : mesh_.vertices())
		{
			auto& v = vertex.vertex();
			for (auto z : grid_z_)
				write_ln(v.x() / mesh_scale, ' ', v.y() / mesh_scale, ' ', z / mesh_scale);
		}
		write_ln();

		// Cells
		const auto n_cells = *mesh_.n_faces() * (grid_z_.size() - 1);
		write_ln("CELLS ", n_cells, ' ', 7 * n_cells);
		for (const auto& face : mesh_.faces())
		{
			std::array<Vertex_index, 3> vertices;
			face.get_indices(vertices);

			for (std::size_t i = 1; i < grid_z_.size(); ++i)
			{
				write(6);
				for (const auto index : vertices)
					write(' ', (i - 1) + grid_z_.size() * *index);
				for (const auto index : vertices)
					write(' ', i + grid_z_.size() * *index);
				write_ln();
			}
		}
		write_ln();

		write_ln("CELL_TYPES ", n_cells);
		for (std::size_t i = 0; i < n_cells; ++i)
			write_ln(VTK_WEDGE);
		write_ln();
	}

	template<class Field>
	void write_scalar_field(const std::string& var_name, const Field& field)
	{
		write_ln("SCALARS ", var_name, " float");
		write_ln("LOOKUP_TABLE default");

		// TODO : use iterators
		// 		for (std::size_t i = 0; i < field.size(); ++i)
		// 			write_ln(field[i]);

		for (std::size_t i = 0; i < *mesh_.n_vertices(); ++i)
			for (std::size_t j = 0; j < grid_z_.size(); ++j)
				write_ln(field(i, j));

		write_ln();
	}

	// 	template<class Field>
	// 	void write_vector_field(const std::string& var_name, const Field& field)
	// 	{
	// 		write_ln("VECTORS ", var_name, " float");
	//
	// 		// TODO : use iterators
	// 		for (std::size_t i = 0; i < field.cols(); ++i)
	// 			write_ln(field(0, i), ' ', field(1, i), ' ', 0);
	//
	// 		write_ln();
	// 	}

private:
	std::ofstream file_;
	const Mesh<2>& mesh_;
	const std::vector<double>& grid_z_;
};
} // namespace esf
