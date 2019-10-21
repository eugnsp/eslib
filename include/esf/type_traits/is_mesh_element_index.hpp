#pragma once
#include <esf/tags.hpp>

#include <esu/type_traits.hpp>

namespace esf
{
template<class T>
inline constexpr bool is_mesh_element_index = 
	esu::is_one_of<T, Vertex_index, Halfedge_index, Edge_index, Face_index>;
} // namespace esf
