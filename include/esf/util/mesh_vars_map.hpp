#pragma once
#include <esf/types.hpp>
#include <esf/util/mesh_element_vars_map.hpp>

#include <esu/algorithm.hpp>
#include <esu/tuple.hpp>

#include <cassert>
#include <cstddef>
#include <tuple>

namespace esf::internal
{
// A class that maps a mesh element index, its type (vertex/edge/face)
// and a variable index into an element of given type:
// (Element_index) x (Var_index) -> (T)
template<class T, class Var_list, class... Element_indices>
class Mesh_var_map_impl
{
public:
	// TODO : unused value
	template<class Mesh>
	void init_storage(const Mesh& mesh, const T& value = T{}, Index n_layers = 1)
	{
		n_layers_ = n_layers;
		esu::tuple_for_each(
			[&mesh, &value, n_layers](auto& list) { list.init_storage(mesh, n_layers, value); },
			maps_);
	}

	template<class Element_index, std::size_t var>
	decltype(auto) at(Element_index element, Var_index<var> v)
	{
		assert(n_layers_ == 1);
		return std::get<Map_t<Element_index>>(maps_).at(element, v);
	}

	template<class Element_index, std::size_t var>
	decltype(auto) at(Element_index element, Var_index<var> v) const
	{
		assert(n_layers_ == 1);
		return std::get<Map_t<Element_index>>(maps_).at(element, v);
	}

	template<class Element_index, std::size_t var>
	decltype(auto) at(Index layer, Element_index element, Var_index<var> v)
	{
		return std::get<Map_t<Element_index>>(maps_).at(element, v, layer, n_layers_);
	}

	template<class Element_index, std::size_t var>
	decltype(auto) at(Index layer, Element_index element, Var_index<var> v) const
	{
		return std::get<Map_t<Element_index>>(maps_).at(element, v, layer, n_layers_);
	}

	// 	template<class Mesh_el_type, std::size_t var>
	// 	T sum(Mesh_el_type, Var_index<var> v) const
	// 	{
	// 		return std::get<Map_t<Mesh_el_type>>(maps_).sum(v);
	// 	}

	template<class Pred>
	bool all_of(Pred pred) const
	{
		return esu::tuple_all_of([&pred](auto& list) { return list.all_of(pred); }, maps_);
	}

	std::size_t memory_size() const
	{
		std::size_t size = 0;
		esu::tuple_for_each([&size](auto& list) { size += list.memory_size(); }, maps_);

		return size;
	}

private:
	template<class Element_index>
	using Map_t = Mesh_element_var_map<T, Element_index, Var_list>;

private:
	std::tuple<Map_t<Element_indices>...> maps_;
	Index n_layers_;
};

template<class Mesh, class Var_list, typename T>
class Mesh_var_map;

template<class Var_list, typename T>
class Mesh_var_map<Mesh<1>, Var_list, T> :
	public Mesh_var_map_impl<T, Var_list, Vertex_index, Edge_index>
{};

template<class Var_list, typename T>
class Mesh_var_map<Mesh<2>, Var_list, T> :
	public Mesh_var_map_impl<T, Var_list, Vertex_index, Edge_index, Face_index>
{};
} // namespace esf::internal
