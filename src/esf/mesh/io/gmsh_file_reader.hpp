#include <esf/mesh/exception.hpp>
#include <esf/mesh/mesh2.hpp>

#include <fstream>
#include <limits>
#include <string>
#include <unordered_map>
#include <vector>

namespace esf::internal
{
class Gmsh_file_reader
{
public:
	Gmsh_file_reader(const std::string& file_name);

	Mesh2 read_mesh(double scale);
	std::vector<unsigned int> read_tags(unsigned int tag_index);

private:
	static constexpr unsigned int GMSH_TRIANGLE = 2;

	template<typename... Ts>
	void read(Ts&... args)
	{
		(file_ >> ... >> args);
		if (file_.bad())
			throw Mesh_io_error("Error reading mesh file");
	}

	void ignore_line()
	{
		file_.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
	}

	void read_mesh_format_section();
	void read_nodes_section(Mesh2&, double scale);
	void read_elements_section(Mesh2&);
	void read_elements_section(std::vector<unsigned int>& tags, unsigned int tag_index);
	void read_and_check_section_footer(const std::string&);

private:
	std::ifstream file_;
	std::unordered_map<Index, Vertex_index> node_indices_map_;
};
} // namespace esf::internal
