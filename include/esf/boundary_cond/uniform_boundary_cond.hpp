#pragma once
#include <esf/boundary_cond/boundary_cond.hpp>

#include <cstddef>

namespace esf
{
template<
	class 	 Element,
	typename T 		    = double,
	bool 	 is_strong_ = true>
class Uniform_boundary_cond : public Boundary_cond<Element>
{
private:
	using Base = Boundary_cond<Element>;

public:
	using Space_dim = typename Base::Space_dim;
	static constexpr bool is_strong  = is_strong_;
	static constexpr bool is_uniform = true;

public:
	using Base::Base;

	Uniform_boundary_cond(
		const Mesh<Space_dim>& mesh,
		const double 		   value)
	:	Base(mesh),
		value_(value)
	{}

	Uniform_boundary_cond(
		const Mesh<Space_dim>&         mesh,
		const typename Base::Boundary& boundary,
		const double 				   value)
	:	Base(mesh, boundary),
		value_(value)
	{}

	template<class Element_index>
	T value(Element_index, std::size_t) const
	{
		return value_;
	}

	void set_value(T value)
	{
		value_ = value;
	}

private:
	T value_;
};
} // namespace esf
