#pragma once
#include <esf/index.hpp>

#include <esl/dense.hpp>

namespace esf
{
template<class Element_, class Quadr_>
class Element_quadr_lagrange
{
private:
	using Element = Element_;
	using Quadr = Quadr_;
	static constexpr auto n_points = Quadr::size;
	static constexpr auto n_dofs = Element::n_total_cell_dofs;

	static_assert(Element::dim == Quadr::dim);

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

template<Local_index order, Local_index dim, class Quadr>
class Element_quadr<Lagrange<order, dim>, Quadr> :
	public Element_quadr_lagrange<Lagrange<order, dim>, Quadr>
{};

template<Local_index order, Local_index dim, class Quadr>
class Element_quadr<Discontinuous_lagrange<order, dim>, Quadr> :
	public Element_quadr_lagrange<Discontinuous_lagrange<order, dim>, Quadr>
{};

} // namespace esf
