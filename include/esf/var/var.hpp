#pragma once
#include <esf/var/var_base.hpp>

#include <esl/dense/tags.hpp>

#include <cstddef>

namespace esf
{
template<class 		 Element,
		 std::size_t ct_dim_ = 1,
		 class... 	 Bnd_conds>
class Var : public internal::Var_base<Element, Bnd_conds...>
{
	static_assert(ct_dim_ > 0);

public:
	static constexpr std::size_t ct_dim = ct_dim_;

private:
	using Base = internal::Var_base<Element, Bnd_conds...>;

public:
	static constexpr std::size_t dim()
	{
		return ct_dim;
	}

	template<class Mesh_element_tag>
	static constexpr std::size_t dofs(Mesh_element_tag tag)
	{
		return ct_dim * Element::dofs(tag);
	}

	template<class Mesh_element_tag>
	static constexpr std::size_t total_dofs(Mesh_element_tag tag)
	{
		return ct_dim * Element::total_dofs(tag);
	}
};

// Class of a variable with dynamic dimension
template<class    Element,
		 class... Bnd_conds>
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

	template<typename Mesh_element_tag>
	std::size_t dofs(Mesh_element_tag tag) const
	{
		return dim_ * Element::dofs(tag);
	}

	template<typename Mesh_element_tag>
	std::size_t total_dofs(Mesh_element_tag tag) const
	{
		return dim_ * Element::total_dofs(tag);
	}

private:
	std::size_t dim_ = 1;
};
} // namespace esf
