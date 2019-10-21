#pragma once
#include <esf/index.hpp>
#include <esf/var/var_base.hpp>

#include <esl/dense/tags.hpp>
#include <esu/tuple.hpp>
#include <esu/type_traits.hpp>

#include <cassert>
#include <cstddef>
#include <memory>
#include <string>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

namespace esf
{
template<class Element, std::size_t ct_dim_ = 1, class... Bnd_conds>
class Var : public internal::Var_base<Element, Bnd_conds...>
{
public:
	static constexpr std::size_t ct_dim = ct_dim_;
	static_assert(ct_dim > 0);

private:
	using Base = internal::Var_base<Element, Bnd_conds...>;

public:
	static constexpr std::size_t dim()
	{
		return ct_dim;
	}

	template<class Tag>
	static constexpr std::size_t n_dofs(Tag tag)
	{
		return ct_dim * Element::dofs(tag);
	}

	template<class Tag>
	static constexpr std::size_t n_total_dofs(Tag tag)
	{
		return ct_dim * Element::total_dofs(tag);
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
	static constexpr std::size_t ct_dim = esl::dynamic;

public:
	std::size_t dim() const
	{
		return dim_;
	}

	void set_dim(std::size_t dim)
	{
		dim_ = dim;
	}

	template<typename Tag>
	std::size_t n_dofs(Tag tag) const
	{
		return dim_ * Element::dofs(tag);
	}

	template<typename Tag>
	std::size_t n_total_dofs(Tag tag) const
	{
		return dim_ * Element::total_dofs(tag);
	}

private:
	std::size_t dim_ = 1;
};
} // namespace esf
