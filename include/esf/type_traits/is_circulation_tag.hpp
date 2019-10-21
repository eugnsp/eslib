#pragma once
#include <esf/tags.hpp>

#include <esu/type_traits.hpp>

namespace esf::internal
{
template<class T>
inline constexpr bool is_circulation_tag =
	esu::is_one_of<T, Vertex_in_circ_tag, Vertex_out_circ_tag, T, Face_circ_tag>;
}
