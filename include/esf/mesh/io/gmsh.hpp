#pragma once
#include <esf/mesh/mesh2.hpp>
#include <esf/types.hpp>

#include <string>
#include <vector>

namespace esf
{
// Reads the given Gmsh mesh file multiplying vertex coordinates
// by the given scale factor
Mesh<2> read_gmsh_mesh(const std::string& file_name, double scale = 1);

// Reads tags from the given Gmsh mesh file with the given tag index
std::vector<unsigned int> read_gmsh_tags(const std::string& file_name, unsigned int tag_index);
} // namespace esf
