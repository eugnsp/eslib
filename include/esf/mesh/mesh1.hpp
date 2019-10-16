#pragma once
#include <esf/geometry/point1.hpp>
#include <esf/mesh/iterator/random_access.hpp>
#include <esf/mesh/view/edge_mesh1.hpp>
#include <esf/mesh/view/vertex_mesh1.hpp>
#include <esf/types.hpp>

#include <esu/iterator.hpp>

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <utility>
#include <vector>
// #include <string>
// #include <iosfwd>

namespace esf
{
template<>
class Mesh<1>
{
public:
	static constexpr std::size_t dim = 1;

public:
	using Cell_index = esf::Edge_index;
	using Vertex_view = Element_view<Vertex_tag, Mesh>;
	using Edge_view = Element_view<Edge_tag, Mesh>;
	using Cell_view = Edge_view;

	using Vertex_iter = Random_access_iterator<Vertex_tag, Mesh>;
	using Edge_iter = Random_access_iterator<Edge_tag, Mesh>;
	using Cell_iter = Edge_iter;

public:
	Mesh() = default;

	Mesh(std::vector<esf::Point1> vertices) : vertices_(std::move(vertices))
	{
		assert(std::is_sorted(vertices_.begin(), vertices_.end()));
	}

	//////////////////////////////////////////////////////////////////////
	//* Capacity */

	esf::Vertex_index n_vertices() const;
	esf::Edge_index n_edges() const;
	Cell_index n_cells() const;

	esf::Vertex_index n_elements(Vertex_tag) const;
	esf::Edge_index n_elements(Edge_tag) const;
	Cell_index n_elements(Cell_tag) const;

	bool is_empty() const;

	//////////////////////////////////////////////////////////////////////

	esf::Vertex_index find_vertex(esf::Point1) const;

	std::pair<esf::Vertex_index, esf::Vertex_index> vertex_indices(esf::Edge_index) const;

	// 	void reserve(Index n_vertices);
	//
	// 	// Returns approximate total size (in bytes) of memory occupied by the mesh
	// 	std::size_t memory_size() const;
	//
	// 	const geom::Point& vertex(Index) const;
	// 	void rescale(double);
	//
	// 	bool is_empty() const;
	// 	bool is_bnd_vertex(Index) const;
	// 	bool is_bnd_cell(Index) const;
	//
	// 	// Performs some basic checks of mesh data structure consistency
	// 	es::util::Error is_ok() const;
	//
	//////////////////////////////////////////////////////////////////////////
	/** Element access */

	const esf::Point1& vertex(esf::Vertex_index vertex) const
	{
		return vertices_[*vertex];
	}

	//////////////////////////////////////////////////////////////////////////
	/** Flags */

	bool is_boundary(esf::Vertex_index) const;
	bool is_boundary(esf::Edge_index) const;

	// 	//////////////////////////////////////////////////////////////////////////
	// 	/** Views */
	//
	// 	Vertex_view vertex_view(Index) const;
	// 	Edge_view edge_view(Index) const;
	//
	//////////////////////////////////////////////////////////////////////////
	/** Iterators */

	Vertex_iter begin_vertex() const;
	Vertex_iter end_vertex() const;
	esu::Iterable<Vertex_iter> vertices() const;

	Edge_iter begin_edge() const;
	Edge_iter end_edge() const;
	esu::Iterable<Edge_iter> edges() const;

	Cell_iter begin_cell() const;
	Cell_iter end_cell() const;
	esu::Iterable<Cell_iter> cells() const;

	//////////////////////////////////////////////////////////////////////////
	//
	// 	std::string type_string() const;
	//
	// private:
	// 	void clear();
	//
	// 	// Adds a new vertex and returns its index
	// 	//Index add_vertex(double);
	//
private:
	std::vector<esf::Point1> vertices_;
};
//
// std::ostream& operator<<(std::ostream&, const Mesh1&);
} // namespace esf
