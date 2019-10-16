#pragma once
#include <esf/element/lagrange/util.hpp>
#include <esf/types.hpp>

#include <esl/dense.hpp>

#include <array>
#include <cassert>

namespace esf::internal
{
template<Local_index order_>
class Lagrange_base_2
{
public:
	static constexpr Local_index order = order_;

public:
	// Returns the value of the basis function (dof) at the given point (point)
	static constexpr double basis(Local_index dof, esl::Vector_2d point)
	{
		const auto r = ijk_by_dof_index(dof);

		const auto x = point[0] * order;
		const auto y = point[1] * order;
		const auto z = order - x - y;

		return prod_frac_xmk_nmk(x, r[0]) * prod_frac_xmk_nmk(y, r[1]) * prod_frac_xmk_nmk(z, r[2]);
	}

	// Returns the value of the basis function (dof) gradient at the given point (point)
	static constexpr esl::Vector_2d basis_grad(Local_index dof, esl::Vector_2d point)
	{
		const auto r = ijk_by_dof_index(dof);

		const auto x = point[0] * order;
		const auto y = point[1] * order;
		const auto z = order - x - y;

		const double px = prod_frac_xmk_nmk(x, r[0]);
		double px_dx = 0;
		for (Local_index p = 0; p < r[0]; ++p)
			px_dx += prod_frac_xmk_nmk(x, r[0], p);

		const double py = prod_frac_xmk_nmk(y, r[1]);
		double py_dy = 0;
		for (Local_index p = 0; p < r[1]; ++p)
			py_dy += prod_frac_xmk_nmk(y, r[1], p);

		const double pz = prod_frac_xmk_nmk(z, r[2]);
		double pz_dz = 0;
		for (Local_index p = 0; p < r[2]; ++p)
			pz_dz += prod_frac_xmk_nmk(z, r[2], p);

		// pz/dx = pz/dy = -pz/dz
		return esl::Vector_2d(py * (px * -pz_dz + pz * px_dx) * order, px * (py * -pz_dz + pz * py_dy) * order);
	}

private:
	static constexpr std::array<Local_index, 3> ijk_by_dof_index(Local_index dof, Local_index order = order_)
	{
		[[maybe_unused]] const auto n_total_dofs = (order + 1) * (order + 2) / 2;
		assert(dof < n_total_dofs && order <= order_);

		if (dof == 0) // Vertex (0, 0)
			return {0, 0, order};

		if (dof == 1) // Vertex (1, 0)
			return {order, 0, 0};

		if (dof == 2) // Vertex (0, 1)
			return {0, order, 0};

		dof -= 2;
		if (dof <= order - 1) // Edge (0, 0) -> (1, 0)
			return {dof, 0, order - dof};

		dof -= order - 1;
		if (dof <= order - 1) // Edge (1, 0) -> (0, 1)
			return {order - dof, dof, 0};

		dof -= order - 1;
		if (dof <= order - 1) // Edge (0, 1) -> (0, 0)
			return {0, order - dof, dof};

		// Internal nodes
		assert(order >= 3);
		dof -= order;

		const auto r = ijk_by_dof_index(dof, order - 3);
		return {r[0] + 1, r[1] + 1, r[2] + 1};
	}
};
} // namespace esf::internal
