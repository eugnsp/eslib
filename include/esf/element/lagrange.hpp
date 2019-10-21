#pragma once
#include <esf/element/element_base.hpp>
#include <esf/element/lagrange/element_quadr_lagrange.hpp>
#include <esf/element/lagrange/lagrange_base_1.hpp>
#include <esf/element/lagrange/lagrange_base_2.hpp>
#include <esf/forward.hpp>
#include <esf/index.hpp>

namespace esf
{
template<std::size_t order>
class Lagrange<order, Dim1> :
	public internal::Element_base<Dim1, 1, order - 1>,
	public internal::Lagrange_base_1<order>
{
	static_assert(order > 0);
};

template<std::size_t order>
class Lagrange<order, Dim2> :
	public internal::Element_base<Dim2, 1, order - 1, (order - 1) * (order - 2) / 2>,
	public internal::Lagrange_base_2<order>
{
	static_assert(order > 0);
};

template<std::size_t order>
class Discontinuous_lagrange<order, Dim1> :
	public internal::Element_base<Dim1, 0, order + 1>,
	public internal::Lagrange_base_1<order>
{};

template<std::size_t order>
class Discontinuous_lagrange<order, Dim2> :
	public internal::Element_base<Dim2, 0, 0, (order + 1) * (order + 2) / 2>,
	public internal::Lagrange_base_2<order>
{};
} // namespace esf
