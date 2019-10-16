#include "gmsh_file_reader.hpp"

#include <esf/mesh/io/gmsh.hpp>
#include <esf/mesh/mesh2.hpp>

#include <string>
#include <vector>

namespace esf
{
Mesh2 read_gmsh_mesh(const std::string& file_name, double scale)
{
	internal::Gmsh_file_reader reader(file_name);

	auto mesh = reader.read_mesh(scale);
	assert(!mesh.check());

	return mesh;
}

std::vector<unsigned int> read_gmsh_tags(const std::string& file_name, unsigned int tag_index)
{
	internal::Gmsh_file_reader reader(file_name);
	return reader.read_tags(tag_index);
}
} // namespace esf
