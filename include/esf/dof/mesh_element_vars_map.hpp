#pragma once
#include <esf/type_traits.hpp>
#include <esf/var/var_list.hpp>

#include <esu/algorithm.hpp>
#include <esu/array.hpp>

#include <algorithm>
#include <array>
#include <cstddef>
#include <vector>

namespace esf::internal
{
// A class that maps a mesh element index and a variable index
// into an element of given type:
// (Element_index) x (Var_index) -> (T)
template<class T, class Element_index, class... Vars>
class Mesh_element_var_map_impl
{
private:
	using Element_tag = Element_tag_by_index<Element_index>;

public:
	template<class Mesh>
	void init_storage(const Mesh& mesh, Index n_layers, const T& value)
	{
		values_.assign(n_layers * block_size() * *mesh.n_elements(Element_tag{}), value);
	}

	template<std::size_t var_idx>
	auto at(Element_index element, Var_index<var_idx>, Index layer = 0, Index n_layers = 1)
		-> typename std::vector<T>::reference
	{
		return values_[layer + n_layers * pos<var_idx>(element)];
	}

	template<std::size_t var_idx>
	auto at(Element_index element, Var_index<var_idx>, Index layer = 0, Index n_layers = 1) const
		-> typename std::vector<T>::const_reference
	{
		return values_[layer + n_layers * pos<var_idx>(element)];
	}

	template<class Pred>
	bool all_of(Pred pred) const
	{
		return std::all_of(values_.begin(), values_.end(), pred);
	}

	std::size_t memory_size() const
	{
		return values_.capacity() * sizeof(T);
	}

private:
	static constexpr auto has_dof_flags =
		esu::make_array(Vars::Element::has_dofs(Element_tag{})...);

	template<std::size_t var_idx>
	static constexpr std::size_t pos(Element_index index)
	{
		static_assert(has_dof_flags[var_idx], "Variable has no DoF of this type");

		constexpr auto offset = esu::array_sum_n<var_idx>(has_dof_flags, std::size_t{0});
		return offset + (*index) * block_size();
	}

	static constexpr std::size_t block_size()
	{
		return esu::array_sum<std::size_t>(has_dof_flags, 0);
	}

private:
	std::vector<T> values_;
};

template<typename T, class Element_index, class Var_list>
class Mesh_element_var_map;

template<typename T, class Element_index, class... Vars>
class Mesh_element_var_map<T, Element_index, Var_list<Vars...>> :
	public Mesh_element_var_map_impl<T, Element_index, Vars...> {};
} // namespace esf::internal
