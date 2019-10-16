#pragma once
#include <esf/geometry/point2.hpp>
#include <esf/geometry/rect.hpp>
#include <esf/mesh/halfedge_structure.hpp>
#include <esf/mesh/iterator/circulator_halfedge_edge.hpp>
#include <esf/mesh/iterator/circulator_vertex_face.hpp>
#include <esf/mesh/iterator/random_access.hpp>
#include <esf/mesh/view/edge_mesh2.hpp>
#include <esf/mesh/view/face_mesh2.hpp>
#include <esf/mesh/view/halfedge_mesh2.hpp>
#include <esf/mesh/view/vertex_mesh2.hpp>
#include <esf/type_traits.hpp>
#include <esf/types.hpp>

#include <esu/error.hpp>
#include <esu/iterator.hpp>

#include <cassert>
#include <cstddef>
#include <iosfwd>
#include <string>
#include <type_traits>
#include <utility>

// enum class MeshObserveCode
// {
// 	BEFORE_REFINE,
// 	REFINE_CELL,
// 	AFTER_REFINE
// };

namespace esf
{
template<>
class Mesh<2> : public internal::Halfedge_structure
{
public:
	static constexpr std::size_t dim = 2;

public:
	using Vertex_view = Element_view<Vertex_tag, Mesh>;
	using Halfedge_view = Element_view<Halfedge_tag, Mesh>;
	using Edge_view = Element_view<Edge_tag, Mesh>;
	using Face_view = Element_view<Face_tag, Mesh>;
	using Cell_view = Face_view;

	using Vertex_iter = Random_access_iterator<Vertex_tag, Mesh>;
	using Halfedge_iter = Random_access_iterator<Halfedge_tag, Mesh>;
	using Edge_iter = Random_access_iterator<Edge_tag, Mesh>;
	using Face_iter = Random_access_iterator<Face_tag, Mesh>;
	using Cell_iter = Face_iter;

	using Boundary_vertex_circ = Circulator<Vertex_tag, Face_circ_tag>;
	using Boundary_halfedge_circ = Circulator<Halfedge_tag, Face_circ_tag>;
	using Boundary_edge_circ = Circulator<Edge_tag, Face_circ_tag>;

private:
	using Base = internal::Halfedge_structure;

public:
	virtual ~Mesh() = default;

	//////////////////////////////////////////////////////////////////////////

	Vertex_index vertex_index_by_halfedge(Halfedge_index) const;

	static Index first_halfedge_index_by_edge(Index);
	static Index second_halfedge_index_by_edge(Index);

	static bool is_first_halfedge(Halfedge_index);
	static bool is_second_halfedge(Halfedge_index);

	Halfedge_index halfedge_index_by_vertex(Vertex_index) const;

	Face_index face_index(Halfedge_index index) const
	{
		return halfedges_[*index].face;
	}

	Halfedge_index halfedge_from_face(Face_index face) const
	{
		return faces_[*face].halfedge;
	}

	virtual Vertex_index find_vertex(const esf::Point2& point) const
	{
		return Base::find_vertex(point);
	}

	/** Returns index of the cell the half-edge belongs to */
	// 				IndexType cell(IndexType edge) const
	// 				{
	// 					return edges_[edge].cell_;
	// 				}

	// Returns global vertex indices on the edge
	std::pair<Index, Index> vertexIndicesOnEdge(Index edge) const
	{
		return {*halfedges_[2 * edge].vertex, *halfedges_[2 * edge + 1].vertex};
	}

	//////////////////////////////////////////////////////////////////////////
	/** Flags */

	//////////////////////////////////////////////////////////////////////////
	/** Views */

	Vertex_view view(Vertex_index) const;
	Halfedge_view view(Halfedge_index) const;
	Edge_view view(Edge_index) const;
	Face_view view(Face_index) const;

	//////////////////////////////////////////////////////////////////////////
	/** Iterators */

	Vertex_iter begin_vertex() const;
	Vertex_iter end_vertex() const;
	esu::Iterable<Vertex_iter> vertices() const;

	Halfedge_iter begin_halfedge() const;
	Halfedge_iter end_halfedge() const;
	esu::Iterable<Halfedge_iter> halfedges() const;

	Edge_iter begin_edge() const;
	Edge_iter end_edge() const;
	esu::Iterable<Edge_iter> edges() const;

	Face_iter begin_face() const;
	Face_iter end_face() const;
	esu::Iterable<Face_iter> faces() const;

	Cell_iter begin_cell() const;
	Cell_iter end_cell() const;
	esu::Iterable<Cell_iter> cells() const;

	Boundary_vertex_circ boundary_vertex_circ() const;
	Boundary_halfedge_circ boundary_halfedge_circ() const;
	Boundary_edge_circ boundary_edge_circ() const;

	//////////////////////////////////////////////////////////////////////////

	// Returns the smallest rectangle that contains the mesh
	Rect bounding_rect() const;

	//////////////////////////////////////////////////////////////////////////

	Index numberOfAdjacentCells(Vertex_index vertex) const
	{
		Index number = 0;
		for_each(
			vertices_[*vertex].halfedge, [&number](auto) { ++number; }, Vertex_out_circ_tag{});

		number -= is_boundary(vertex);

		assert(number > 0);
		return number;
	}

	virtual std::string type_string() const
	{
		return "2D unstructured mesh";
	};

	// Performs some basic checks of mesh data structure consistency
	esu::Error check() const;

	// Clears edges and cells preserving vertices
	void clearEdgesAndCells()
	{
		halfedges_.clear();
		faces_.clear();
	}

	void scale(double scale)
	{
		for (auto& v : vertices_)
			v.point *= scale;
	}

	// Outputs human readable information about the mesh
	void print(std::ostream&) const;

private:
	Halfedge_index outgoing_halfedge(Vertex_index vertex) const
	{
		return vertices_[*vertex].halfedge;
	}

	Halfedge_index ingoing_halfedge(Vertex_index vertex) const
	{
		return twin(outgoing_halfedge(vertex));
	}
};

// Outputs human readable information about the mesh
inline std::ostream& operator<<(std::ostream& os, const Mesh<2>& mesh)
{
	mesh.print(os);
	return os;
}
} // namespace esf
