#pragma once
#include <esf/types.hpp>
#include <esf/var_list.hpp>

#include <esu/type_traits.hpp>

#include <type_traits>

namespace esf::internal
{
template<class T>
inline constexpr bool is_circulation_tag =
	std::is_same_v<T, Vertex_in_circ_tag> || std::is_same_v<T, Vertex_out_circ_tag> ||
	std::is_same_v<T, Face_circ_tag>;

//////////////////////////////////////////////////////////////////////////
// Element tag type by index type, and vice versa

template<class Element_tag>
struct Element_index_by_tag_impl;

template<>
struct Element_index_by_tag_impl<Vertex_tag>
{
	using Type = Vertex_index;
};

template<>
struct Element_index_by_tag_impl<Halfedge_tag>
{
	using Type = Halfedge_index;
};

template<>
struct Element_index_by_tag_impl<Edge_tag>
{
	using Type = Edge_index;
};

template<>
struct Element_index_by_tag_impl<Face_tag>
{
	using Type = Face_index;
};

template<class Element_index>
struct Element_tag_by_index_impl;

template<>
struct Element_tag_by_index_impl<Vertex_index>
{
	using Type = Vertex_tag;
};

template<>
struct Element_tag_by_index_impl<Halfedge_index>
{
	using Type = Halfedge_tag;
};

template<>
struct Element_tag_by_index_impl<Edge_index>
{
	using Type = Edge_tag;
};

template<>
struct Element_tag_by_index_impl<Face_index>
{
	using Type = Face_tag;
};

template<class Element_tag>
using Element_index_by_tag = typename Element_index_by_tag_impl<Element_tag>::Type;

template<class Element_tag>
using Element_tag_by_index = typename Element_tag_by_index_impl<Element_tag>::Type;

//////////////////////////////////////////////////////////////////////////
} // namespace esf::internal

namespace esf
{
template<std::size_t var>
using Var_index = esu::Index<var>;

template<class Var_list, class Var_index>
struct Var_by_var_index_impl;

template<class Var_list, std::size_t index>
struct Var_by_var_index_impl<Var_list, Var_index<index>>
{
	using Type = typename Var_list::template Nth<index>;
};

template<class Var_list, class Var_index>
using Var_by_var_index =
	typename Var_by_var_index_impl<Var_list, esu::Remove_cv_ref<Var_index>>::Type;

} // namespace esf

namespace esf::internal
{
template<class T>
struct Is_var_list : std::false_type
{};

template<class... Vars>
struct Is_var_list<Var_list<Vars...>> : std::true_type
{};

template<class Var_or_var_list>
using Wrap_into_var_list_t = std::conditional_t<Is_var_list<Var_or_var_list>::value,
	Var_or_var_list, Var_list<Var_or_var_list>>;

template<class System>
struct System_traits;

template<class T_Var_list, template<class> class T_Dof_mapper>
struct System_traits<System<T_Var_list, T_Dof_mapper>>
{
	using Var_list = T_Var_list;
};

template<class Tag>
struct Is_vertex_tag : std::is_same<Tag, Vertex_tag>
{};

template<class Tag>
constexpr bool is_vertex_tag_v = Is_vertex_tag<Tag>::value;

template<class Tag>
struct Is_edge_tag : std::is_same<Tag, Edge_tag>
{};

template<class Tag>
constexpr bool is_edge_tag_v = Is_edge_tag<Tag>::value;

////////////////////////////////////////////////////////////////

template<class Element_index>
struct Decay_element_in_view_index_impl
{
	using Type = Element_index;
};

template<>
struct Decay_element_in_view_index_impl<Edge_index>
{
	using Type = Halfedge_index;
};

template<class Element_index, class Mesh>
struct Decay_element_index_in_view_impl
{
	using Type = Element_index;
};

template<class Element_index>
struct Decay_element_index_in_view_impl<Element_index, Mesh2>
{
	using Type = std::conditional_t<std::is_same_v<Element_index, Edge_index>, Halfedge_index,
		Element_index>;
};

// For Edge_index returns Halfedge_index,
// for other index types returns them intact
template<class Element_index, class Mesh>
using Decay_element_index_in_view =
	typename Decay_element_index_in_view_impl<Element_index, Mesh>::Type;

//////////////////////////////////////////////////////////////////////////

template<class Element_tag, class Circ_tag>
struct Circulator_type_selector;

template<class Circ_tag>
struct Circulator_type_selector<Vertex_tag, Circ_tag>
{
	using Type = Circulator_vertex_face<Vertex_tag, Circ_tag>;
};

template<class Circ_tag>
struct Circulator_type_selector<Face_tag, Circ_tag>
{
	using Type = Circulator_vertex_face<Face_tag, Circ_tag>;
};

template<class Circ_tag>
struct Circulator_type_selector<Halfedge_tag, Circ_tag>
{
	using Type = Circulator_halfedge_edge<Halfedge_tag, Circ_tag>;
};

template<class Circ_tag>
struct Circulator_type_selector<Edge_tag, Circ_tag>
{
	using Type = Circulator_halfedge_edge<Edge_tag, Circ_tag>;
};
} // namespace esf::internal

namespace esf
{
template<class Element_tag, class Circ_tag>
using Circulator = typename internal::Circulator_type_selector<Element_tag, Circ_tag>::Type;

}
