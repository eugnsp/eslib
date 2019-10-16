#include <esf/mesh/mesh1.hpp>
#include <esf/types.hpp>

namespace esf
{
auto Mesh1::begin_vertex() const -> Vertex_iter
{
	return {*this, Vertex_index{0}};
}

auto Mesh1::end_vertex() const -> Vertex_iter
{
	return {*this, n_vertices()};
}

auto Mesh1::vertices() const -> esu::Iterable<Vertex_iter>
{
	return {begin_vertex(), end_vertex()};
}

auto Mesh1::begin_edge() const -> Edge_iter
{
	return {*this, Edge_index{0}};
}

auto Mesh1::end_edge() const -> Edge_iter
{
	return {*this, n_edges()};
}

auto Mesh1::edges() const -> esu::Iterable<Edge_iter>
{
	return {begin_edge(), end_edge()};
}

auto Mesh1::begin_cell() const -> Cell_iter
{
	return begin_edge();
}

auto Mesh1::end_cell() const -> Cell_iter
{
	return end_edge();
}

auto Mesh1::cells() const -> esu::Iterable<Cell_iter>
{
	return edges();
}
} // namespace esf
