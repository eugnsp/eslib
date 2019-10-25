#pragma once
#include <esf/forward.hpp>

#include <esl/dense.hpp>

#include <cstddef>

namespace esf
{
// Returns a local mass matrix
template<class Element, class Quadr = esf::Quadr<2 * Element::order, typename Element::Space_dim>,
	     class Fn>
auto mass_matrix(Fn fn, const double scale)
	-> esl::Matrix_d<Element::total_cell_dofs, Element::total_cell_dofs>
{
	constexpr auto n_dofs = Element::total_cell_dofs;
	return esl::make_matrix<n_dofs, n_dofs>(
		[&fn, scale](std::size_t row, std::size_t col)
		{
			return scale * Quadr::sum([row, col, &fn](std::size_t iq)
			{
				constexpr auto basis = Element_quadr<Element, Quadr>::basis();
				return fn(iq) * basis(iq, row) * basis(iq, col);
			});
		});
}

// Returns a local mass matrix
template<class Element, class Quadr = esf::Quadr<2 * Element::order, typename Element::Space_dim>>
auto mass_matrix(const double scale)
	-> esl::Matrix_d<Element::total_cell_dofs, Element::total_cell_dofs>
{
	constexpr auto n_dofs = Element::total_cell_dofs;
	constexpr auto mat = esl::make_matrix<n_dofs, n_dofs>(
		[](std::size_t row, std::size_t col)
		{
			return Quadr::sum([row, col](std::size_t iq)
			{
				constexpr auto basis = Element_quadr<Element, Quadr>::basis();
				return basis(iq, row) * basis(iq, col);
			});
		});
	return scale * mat;
}
} // namespace esf
