#pragma once
#include <esf/element/lagrange/prod_frac.hpp>
#include <esf/geometry/point1.hpp>

#include <array>
#include <cassert>
#include <cstddef>

namespace esf::internal
{
template<std::size_t order_>
class Lagrange_base_1
{
public:
	static constexpr std::size_t order = order_;

public:
	// Returns the value of the basis function (dof) at the given point (point)
	static constexpr double basis(const std::size_t dof, const Point1 point)
	{
		const auto r = ij_by_dof_index(dof);

		const auto x = point.x() * order;
		return prod_frac_xmk_nmk(x, r[0]) * prod_frac_xmk_nmk(order - x, r[1]);
	}

	// Returns the value of the basis function (dof) gradient at the given point (point)
	static constexpr double basis_grad(const std::size_t dof, const Point1 point)
	{
		const auto r = ij_by_dof_index(dof);

		const auto x = point.x() * order;
		const double px = prod_frac_xmk_nmk(x, r[0]);
		double px_dx = 0;
		for (std::size_t p = 0; p < r[0]; ++p)
			px_dx += prod_frac_xmk_nmk(x, r[0], p);

		const double pz = prod_frac_xmk_nmk(order - x, r[1]);
		double pz_dz = 0;
		for (std::size_t p = 0; p < r[1]; ++p)
			pz_dz += prod_frac_xmk_nmk(order - x, r[1], p);

		// pz/dx = -pz/dz
		return (px * -pz_dz + pz * px_dx) * order;
	}

private:
	static constexpr std::array<std::size_t, 2> ij_by_dof_index(std::size_t dof)
	{
		[[maybe_unused]] const auto n_total_dofs = order + 1;
		assert(dof < n_total_dofs);

		if (dof == 0)
			return {0, order};

		if (dof == 1)
			return {order, 0};

		--dof;
		return {dof, order - dof};
	}
};
} // namespace esf::internal
