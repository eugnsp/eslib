#pragma once
#include <esf/mesh/mesh2.hpp>
#include <esf/types.hpp>
#include <esu/time.hpp>

#include <array>
#include <cassert>
#include <fstream>
#include <stdexcept>
#include <string>

namespace esf
{
class Vtk_writer
{
private:
	enum class Data_section
	{
		UNDEF,
		POINT_DATA,
		CELL_DATA
	};

public:
	Vtk_writer(const std::string& file_name, const Mesh<2>& mesh, double mesh_scale = 1) :
		file_(file_name), mesh_(mesh)
	{
		if (!file_)
			throw std::runtime_error("File '" + file_name + "' cannot be opened for writing");

		write_header();
		write_mesh(mesh_scale);
	}

	template<class Field>
	void write_vertex_scalar_field(const std::string& var_name, const Field& field)
	{
		assert(field.size() == mesh_.n_vertices());

		write_data_section_header(Data_section::POINT_DATA);
		write_scalar_field(var_name, field);
	}

	template<class Field>
	void write_vertex_vector_field(const std::string& var_name, const Field& field)
	{
		assert(field.rows() == 2 && field.cols() == mesh_.n_vertices());

		write_data_section_header(Data_section::POINT_DATA);
		write_vector_field(var_name, field);
	}

	template<class Field>
	void write_face_scalar_field(const std::string& var_name, const Field& field)
	{
		assert(field.size() == *mesh_.n_faces());

		write_data_section_header(Data_section::CELL_DATA);
		write_scalar_field(var_name, field);
	}

	template<class Field>
	void write_face_vector_data(const std::string& var_name, const Field& data)
	{
		assert(data.rows() == 2 && data.cols() == *mesh_.n_faces());

		write_data_section_header(Data_section::CELL_DATA);
		write_vector_field(var_name, data);
	}

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
		write_ln("DATASET UNSTRUCTURED_GRID");
		write_ln("POINTS ", *mesh_.n_vertices(), " double");

		// Vertices
		for (const auto& vertex : mesh_.vertices())
		{
			auto& v = vertex.vertex();
			write_ln(v.x() / mesh_scale, ' ', v.y() / mesh_scale, " 0");
		}
		write_ln();

		// Faces
		std::vector<unsigned int> cell_types(*mesh_.n_faces());
		auto cell_list_size = *mesh_.n_faces(); // Cells' indices

		// TODO :: all triangles
		for (const auto& face : mesh_.faces())
		{
			const auto n = 3;
			cell_types[**face] = get_vtk_element_type(3);
			cell_list_size += 3;
		}

		write_ln("CELLS ", *mesh_.n_faces(), ' ', cell_list_size);
		for (const auto& face : mesh_.faces())
		{
			std::array<Vertex_index, 3> vertices;
			face.get_indices(vertices);

			write(vertices.size());
			for (const auto index : vertices)
				write(' ', *index);

			write_ln();
		}
		write_ln();

		write_ln("CELL_TYPES ", *mesh_.n_faces());
		for (auto type : cell_types)
			write_ln(type);
		write_ln();
	}

	void write_data_section_header(Data_section section)
	{
		if (last_section_ == section)
			return;

		switch (section)
		{
		case Data_section::POINT_DATA:
			write_ln("POINT_DATA ", *mesh_.n_vertices());
			break;

		case Data_section::CELL_DATA:
			write_ln("CELL_DATA ", *mesh_.n_faces());
			break;

		default:
			assert(false);
		}

		last_section_ = section;
	}

	template<class Field>
	void write_scalar_field(const std::string& var_name, const Field& field)
	{
		write_ln("SCALARS ", var_name, " double");
		write_ln("LOOKUP_TABLE default");

		// TODO : use iterators
		for (std::size_t i = 0; i < field.size(); ++i)
			write_ln(+field[i]); // "+" to promote to a type printable as number

		write_ln();
	}

	template<class Field>
	void write_vector_field(const std::string& var_name, const Field& field)
	{
		write_ln("VECTORS ", var_name, " double");

		// TODO : use iterators
		for (std::size_t i = 0; i < field.cols(); ++i)
			write_ln(field(0, i), ' ', field(1, i), ' ', 0);

		write_ln();
	}

	static constexpr unsigned int get_vtk_element_type(std::size_t n_vertices)
	{
		constexpr unsigned int VTK_TRIANGLE = 5;
		constexpr unsigned int VTK_QUAD = 9;
		constexpr unsigned int VTK_POLYGON = 7;

		if (n_vertices == 3)
			return VTK_TRIANGLE;
		if (n_vertices == 4)
			return VTK_QUAD;
		return VTK_POLYGON;
	}

private:
	std::ofstream file_;
	const Mesh<2>& mesh_;
	Data_section last_section_ = Data_section::UNDEF;
};
} // namespace esf
