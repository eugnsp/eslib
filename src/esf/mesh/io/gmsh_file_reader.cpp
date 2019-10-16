#include "gmsh_file_reader.hpp"

#include <esf/mesh/exception.hpp>
#include <esf/mesh/mesh2.hpp>

#include <fstream>
#include <memory>
#include <string>
#include <vector>

namespace esf::internal
{
Gmsh_file_reader::Gmsh_file_reader(const std::string& file_name) : file_(file_name)
{
	if (!file_)
		throw Mesh_io_error("Mesh file '" + file_name + "' cannot be opened for reading");
}

Mesh2 Gmsh_file_reader::read_mesh(double scale)
{
	file_.clear();
	file_.seekg(0, std::ifstream::beg);

	Mesh2 mesh;

	std::string line;
	while (std::getline(file_, line))
	{
		if (line.find("$MeshFormat") == 0)
			read_mesh_format_section();
		else if (line.find("$Nodes") == 0)
			read_nodes_section(mesh, scale);
		else if (line.find("$Elements") == 0)
			read_elements_section(mesh);
	}

	mesh.shrink();
	return mesh;
}

std::vector<unsigned int> Gmsh_file_reader::read_tags(unsigned int tag_index)
{
	file_.clear();
	file_.seekg(0, std::ifstream::beg);

	std::vector<unsigned int> tags;

	std::string line;
	while (std::getline(file_, line))
	{
		if (line.find("$MeshFormat") == 0)
			read_mesh_format_section();
		else if (line.find("$Elements") == 0)
			read_elements_section(tags, tag_index);
	}

	return tags;
}

void Gmsh_file_reader::read_mesh_format_section()
{
	std::string version;
	unsigned int file_type, data_size;
	read(version, file_type, data_size);

	if (version != "2.2")
		throw Mesh_io_error("Unsupported Gmsh mesh file version " + version);

	if (file_type != 0 || data_size != 8)
		throw Mesh_io_error("Unsupported Gmsh mesh file format");

	read_and_check_section_footer("$EndMeshFormat");
}

void Gmsh_file_reader::read_nodes_section(Mesh2& mesh, double scale)
{
	Index n_vertices;
	read(n_vertices);

	mesh.reserve(n_vertices);
	node_indices_map_.reserve(n_vertices);

	for (Index i = 0; i < n_vertices; ++i)
	{
		Index index;
		double x, y, z;
		read(index, x, y, z);

		if (z != 0)
			throw Mesh_io_error("Not 2D Gmsh mesh file");

		const auto vertex_index = mesh.add_vertex({x * scale, y * scale});
		node_indices_map_.insert({index, vertex_index});
	}

	read_and_check_section_footer("$EndNodes");
}

void Gmsh_file_reader::read_elements_section(Mesh2& mesh)
{
	Index n_elements;
	read(n_elements);

	for (Index i = 0; i < n_elements; ++i)
	{
		Index index;
		unsigned int element_type;
		read(index, element_type);

		if (element_type != GMSH_TRIANGLE)
		{
			ignore_line();
			continue;
		}

		unsigned int n_tags;
		read(n_tags);

		for (unsigned int t = 0; t < n_tags; ++t)
		{
			unsigned int ignore;
			file_ >> ignore;
		}

		Index v1, v2, v3;
		read(v1, v2, v3);
		const auto vi1 = node_indices_map_.at(v1);
		const auto vi2 = node_indices_map_.at(v2);
		const auto vi3 = node_indices_map_.at(v3);

		mesh.add_cell({vi1, vi2, vi3});
	}

	read_and_check_section_footer("$EndElements");
}

void Gmsh_file_reader::read_elements_section(
	std::vector<unsigned int>& tags, unsigned int tag_index)
{
	Index n_elements;
	read(n_elements);

	for (Index i = 0; i < n_elements; ++i)
	{
		Index index;
		unsigned int element_type;
		read(index, element_type);

		if (element_type != GMSH_TRIANGLE)
		{
			ignore_line();
			continue;
		}

		unsigned int n_tags;
		read(n_tags);

		if (tag_index >= n_tags)
			throw Mesh_io_error("Bad tag index");

		unsigned int tag;
		for (unsigned int t = 0; t < tag_index; ++t)
			file_ >> tag;

		file_ >> tag;
		tags.push_back(tag);
		ignore_line();
	}

	read_and_check_section_footer("$EndElements");
}

void Gmsh_file_reader::read_and_check_section_footer(const std::string& footer)
{
	std::string section_footer;
	read(section_footer);

	if (section_footer != footer)
		throw Mesh_io_error("Bad Gmsh mesh file");
}
} // namespace esf::internal
