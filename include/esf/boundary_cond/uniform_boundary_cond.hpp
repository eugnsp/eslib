#pragma once
#include <esf/boundary_cond/boundary_cond.hpp>

namespace esf
{
template<class Element, typename T = double, bool is_essential_ = true>
class Uniform_boundary_cond : public Boundary_cond<Element, is_essential_>
{
public:
	static constexpr bool is_essential = is_essential_;
	static constexpr bool is_uniform = true;

private:
	using Base = Boundary_cond<Element, is_essential_>;

public:
	template<class Arg>
	Uniform_boundary_cond(const Mesh<Element::dim>& mesh, const Arg& arg) : Base(mesh, arg)
	{}

	template<class Arg>
	Uniform_boundary_cond(const Mesh<Element::dim>& mesh, const Arg& arg, double value) :
		Base(mesh, arg), value_(value)
	{}

	template<class... Ignore>
	T value(const Ignore&...) const
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
