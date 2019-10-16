#pragma once
#include <esf/types.hpp>

#include <esl/dense.hpp>

namespace esf
{
template<esf::Local_index order, esf::Local_index dim, class Quadr>
class Element_quadr<esf::Lagrange<order, dim>, Quadr>
{
private:
	using Element = esf::Lagrange<order, dim>;
	static constexpr auto n_points = Quadr::size;
	static constexpr auto n_dofs = Element::n_total_cell_dofs;

public:
	// Returns the values of basis functions at the quadrature points
	// as the matrix of the size (# quadr. points) x (# dofs)
	static constexpr auto basis()
	{
		return esl::make_matrix<n_points, n_dofs>([](
			auto quadr, auto dof) constexpr { return Element::basis(dof, Quadr::point(quadr)); });
	}

	// Returns the values of gradients of basis functions at the quadrature points
	// as the matrix of the size (# quadr. points) x (# dofs)
	static constexpr auto basis_grad()
	{
		return esl::make_matrix<n_points, n_dofs>([](auto quadr, auto dof) constexpr {
			return Element::basis_grad(dof, Quadr::point(quadr));
		});
	}
};

template<esf::Local_index order, esf::Local_index dim, class Quadr>
class Element_quadr<esf::Discontinuous_lagrange<order, dim>, Quadr> :
	public Element_quadr<esf::Lagrange<order, dim>, Quadr>
{};
} // namespace esf
