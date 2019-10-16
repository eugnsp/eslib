#pragma once
#include <esf/types.hpp>
#include <esf/var_base.hpp>

#include <esu/tuple.hpp>
#include <esu/type_traits.hpp>

#include <cassert>
#include <memory>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

namespace esf
{
// Class of a variable with static dimension
//
// Template parameters:
//		Element 		- finite element type,
//		dim				- dimension of a variable (positive),
//		Bnd_conds... 	- zero of more boundary condition types.
template<class Element, Local_index dim_ = 1, class... Bnd_conds>
class Var : public internal::Var_base<Element, Bnd_conds...>
{
	static_assert(dim_ > 0);

private:
	using Base = internal::Var_base<Element, Bnd_conds...>;

public:
	static constexpr Local_index dim()
	{
		return dim_;
	}

	template<class Tag>
	static constexpr Local_index n_dofs(Tag tag)
	{
		return dim() * Element::n_dofs(tag);
	}

	template<class Tag>
	static constexpr Local_index n_total_dofs(Tag tag)
	{
		return dim() * Element::n_total_dofs(tag);
	}
};

// Class of a variable with dynamic dimension
//
// Template parameters:
//		Element 		- finite element type,
//		Bnd_conds... 	- zero of more boundary condition types.
template<class Element, class... Bnd_conds>
class Var_x : public internal::Var_base<Element, Bnd_conds...>
{
public:
	Local_index dim() const
	{
		return dim_;
	}

	void set_dim(Local_index dim)
	{
		dim_ = dim;
	}

	template<typename Tag>
	Local_index n_dofs(Tag tag) const
	{
		return dim() * Element::n_dofs(tag);
	}

	template<typename Tag>
	Local_index n_total_dofs(Tag tag) const
	{
		return dim() * Element::n_total_dofs(tag);
	}

private:
	Local_index dim_ = 1;
};
} // namespace esf
