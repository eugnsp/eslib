#pragma once
#include <esf/boundary_cond/boundary_cond_base1.hpp>
#include <esf/boundary_cond/boundary_cond_base2.hpp>

namespace esf
{
template<class Element, bool is_essential_ = true>
class Boundary_cond : public internal::Boundary_cond_base<Element::dim, Element>
{
private:
	using Base = internal::Boundary_cond_base<Element::dim, Element>;

public:
	static constexpr bool is_essential = is_essential_;
	static constexpr bool is_uniform = false;

public:
	using Base::Base;
};
} // namespace esf
