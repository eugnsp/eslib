#pragma once
#include <esf/index.hpp>

#include <esu/enum_class_index.hpp>

namespace esf
{
#define ESF_DEFINE_INDEX_TYPE(Index_name, Literal_name)                                            \
	ESU_ENUM_CLASS_INDEX_TYPE(Index, Index_name, invalid = invalid_index)                          \
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

ESF_DEFINE_INDEX_TYPE(Vertex_index, vi)
ESF_DEFINE_INDEX_TYPE(Halfedge_index, hi)
ESF_DEFINE_INDEX_TYPE(Edge_index, ei)
ESF_DEFINE_INDEX_TYPE(Face_index, fi)
} // namespace esf
