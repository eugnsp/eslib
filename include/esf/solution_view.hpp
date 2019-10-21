#pragma once
#include <esf/system/dofs.hpp>
#include <esf/function.hpp>
#include <esf/geometry.hpp>
#include <esf/index.hpp>

#include <esl/dense.hpp>

#include <cassert>
#include <cstddef>
#include <type_traits>
#include <utility>

namespace esf
{
template<class System_, std::size_t var_idx, typename Value>
class Solution_view
{
public:
	using System = System_;
	using Mesh = typename System::Mesh;

private:
	using Var = typename System::template Var_t<var_idx>;
	using Element = typename Var::Element;
	static constexpr Var_index<var_idx> var_index{};

public:
	Solution_view(const System& system, const esl::Vector_x<Value>& solution) :
		system_(system), solution_(solution)
	{}

	///////////////////////////////////////////////////////////////////////
	/** Solution values access */

	template<class Quadr>
	auto at_quadr(const typename System::template Var_dofs<var_idx>& dofs) const
	{
		esl::Vector<Value, Quadr::size> vals_at_quadr{};

		for (std::size_t iq = 0; iq < Quadr::size; ++iq)
			for (std::size_t id = 0; id < dofs.size(); ++id)
			{
				const auto v = solution_[dofs[id].index];
				vals_at_quadr[iq] += Element_quadr<Element, Quadr>::basis()(iq, id) * v;
			}

		return vals_at_quadr;
	}

	template<class Quadr>
	auto at_quadr(const typename System::Mesh::Cell_view& cell) const
	{
		const auto dofs = system_.dof_mapper().template dofs<var_idx>(cell);
		return at_quadr<Quadr>(dofs);
	}

	auto& at(Vertex_index vertex) const
	{
		typename System::template Var_vertex_dofs<var_idx> dofs;
		system_.dof_mapper().template vertex_dofs<var_idx>(vertex, dofs);

		// TODO : return a vector if we have several DoFs
		return solution_[dofs[0].index];
	}

	auto at(Face_index face) const
	{
		const auto& var = system_.variable(var_index);
		const auto dofs = esf::dofs(system_, var_index, face);

		if constexpr (Var::ct_dim == 1 && Element::face_dofs == 1)
			return solution_[dofs[0]];
		else
		{
			esl::Matrix<Value, Var::ct_dim, Element::face_dofs> values;
			values.resize(var.dim(), Element::face_dofs);
			for (std::size_t dim = 0; dim < var.dim(); ++dim)
				for (std::size_t dof = 0; dof < Element::face_dofs; ++dof)
					values(dim, dof) = solution_[dofs[0].index + dim + dof * var.dim()];

			return values;
		}
	}

	auto& operator[](Vertex_index vertex) const
	{
		return at(vertex);
	}

	Value at(const esf::Point2& pt, const typename Mesh::Cell_view& cell) const
	{
		static_assert(Mesh::dim == 2);

		const auto pt_ref = point_to_ref_triangle(pt, cell);
		const auto dofs = system_.dof_mapper().template dofs<var_idx>(cell);
		Value value{};
		for (Local_index id = 0; id < dofs.size(); ++id)
			value += Element::basis(id, pt_ref) * solution_[dofs[id].index];
		return value;
	}

	Value operator()(const esf::Point2& pt, const typename Mesh::Cell_view& cell) const
	{
		return at(pt, cell);
	}

	const System& system() const
	{
		return system_;
	}

	const typename System::Mesh& mesh() const
	{
		return system_.mesh();
	}

protected:
	const System& system_;
	const esl::Vector_x<Value>& solution_;
};

template<class Quadr, class System, std::size_t var, typename Value>
auto at_quadr(const Solution_view<System, var, Value>& solution,
	const typename System::template Var_dofs<var>& dofs)
{
	return solution.template at_quadr<Quadr>(dofs);
}

template<class Quadr, class System, std::size_t var, typename Value>
auto at_quadr(
	const Solution_view<System, var, Value>& solution, const typename System::Mesh::Cell_view& cell)
{
	return solution.template at_quadr<Quadr>(cell);
}
} // namespace esf
