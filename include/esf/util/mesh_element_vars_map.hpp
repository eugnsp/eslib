#pragma once
#include <esf/type_traits.hpp>
#include <esf/types.hpp>
#include <esf/var_list.hpp>

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
template<typename T, class Element_index, class... Vars>
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

	template<std::size_t var>
	typename std::vector<T>::reference at(
		Element_index element, Var_index<var>, Index layer = 0, Index n_layers = 1)
	{
		return values_[layer + n_layers * pos<var>(element)];
	}

	template<std::size_t var>
	typename std::vector<T>::const_reference at(
		Element_index element, Var_index<var>, Index layer = 0, Index n_layers = 1) const
	{
		return values_[layer + n_layers * pos<var>(element)];
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

	template<std::size_t var>
	static constexpr std::size_t pos(Element_index element)
	{
		static_assert(has_dof_flags[var], "Variable has no DoF of this type");

		constexpr auto offset = esu::array_sum_n<var, std::size_t>(has_dof_flags, 0);
		return offset + (*element) * block_size();
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
	public Mesh_element_var_map_impl<T, Element_index, Vars...>
{};
} // namespace esf::internal
