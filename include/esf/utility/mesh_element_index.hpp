#pragma once
#include <esf/index/mesh_element_index.hpp>

namespace esf
{
inline Halfedge_index twin(Halfedge_index index)
{
	return Halfedge_index{*index ^ 1};
}

inline Edge_index edge(Halfedge_index index)
{
	return Edge_index{*index / 2};
}

inline Halfedge_index first_halfedge(Edge_index index)
{
	return Halfedge_index{2 * *index};
}

inline Halfedge_index second_halfedge(Edge_index index)
{
	return first_halfedge(index) + 1;
}

inline bool is_first_halfedge(Halfedge_index index)
{
	return *index % 2 == 0;
}

inline bool is_second_halfedge(Halfedge_index index)
{
	return !is_first_halfedge(index);
}
} // namespace esf
