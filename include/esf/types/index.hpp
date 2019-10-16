#pragma once
#include <esu/enum_class_index.hpp>

#include <cassert>
#include <cstdint>
#include <utility>

namespace esf
{
using Index = std::uint32_t;
using Local_index = unsigned int;

inline constexpr auto invalid_index = static_cast<Index>(-1);
inline constexpr auto invalid_local_index = static_cast<Local_index>(-1);

template<Local_index... indices>
using Local_index_sequence = std::integer_sequence<Local_index, indices...>;

template<Local_index n>
using Make_local_index_sequence = std::make_integer_sequence<Local_index, n>;

#define ES_FE_DEFINE_INDEX_TYPE(Index_name, Literal_name)                                          \
	ES_UTIL_ENUM_CLASS_INDEX_TYPE(Index, Index_name, invalid = invalid_index)                      \
                                                                                                   \
	inline bool is_valid(Index_name index)                                                         \
	{                                                                                              \
		return index != Index_name::invalid;                                                       \
	}                                                                                              \
                                                                                                   \
	namespace literals                                                                             \
	{                                                                                              \
	inline constexpr auto operator"" _##Literal_name(unsigned long long index)                     \
	{                                                                                              \
		return static_cast<Index_name>(index);                                                     \
	}                                                                                              \
	}

ES_FE_DEFINE_INDEX_TYPE(Vertex_index, vi)
ES_FE_DEFINE_INDEX_TYPE(Halfedge_index, hi)
ES_FE_DEFINE_INDEX_TYPE(Edge_index, ei)
ES_FE_DEFINE_INDEX_TYPE(Face_index, fi)

// TODO : move to somewhere else
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
