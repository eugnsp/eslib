#pragma once
#include <esf/dof/function.hpp>
#include <esf/function.hpp>
#include <esf/geometry.hpp>
#include <esf/index.hpp>
#include <esf/type_traits.hpp>

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
	using Mesh   = typename System::Mesh;

private:
	using Var     = Var_type<System, var_idx>;
	using Element = typename Var::Element;
	static constexpr Var_index<var_idx> var_index{};

public:
	Solution_view(const System& system, const esl::Vector_x<Value>& solution)
		: system_(system), solution_(solution)
	{}

	////////////////////////////////////////////////////////////////////////////////////////////////
	/** Solution values access */

	template<class Quadr>
	auto at_quadr(const typename System::Dof_mapper::template Var_dofs<var_idx, Cell_tag>& dofs) const
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
	auto at_quadr(const typename Mesh::Cell_view& cell) const
	{
		const auto dofs = system_.dof_mapper().template dofs<var_idx>(cell);
		return at_quadr<Quadr>(dofs);
	}

	template<class Mesh_element_index,
			 class Mesh_element_tag = internal::Element_tag_by_index<Mesh_element_index>>
	auto at(Mesh_element_index mesh_element_index) const
	{
		static_assert(Element::has_dofs(Mesh_element_tag{}));
		constexpr auto n_dofs = Element::dofs(Mesh_element_tag{});

		const auto& var = system_.variable(var_index);
		const auto dofs = esf::dofs(system_, mesh_element_index, var_index);
		assert(dofs.size() == n_dofs);

		esl::Matrix<Value, Var::ct_dim, n_dofs> values;
		values.resize(var.dim(), n_dofs);
		for (std::size_t dim = 0; dim < var.dim(); ++dim)
			for (std::size_t dof = 0; dof < n_dofs; ++dof)
				values(dim, dof) = solution_[dofs[dof].index + dim];

		return values;
	}

	auto at(const typename Mesh::Cell_view& cell) const
	{
		constexpr auto n_dofs = Element::total_cell_dofs;

		const auto& var = system_.variable(var_index);
		const auto dofs = esf::dofs(system_, cell, var_index);
		assert(dofs.size() == n_dofs);

		esl::Matrix<Value, Var::ct_dim, n_dofs> values;
		values.resize(var.dim(), n_dofs);
		for (std::size_t dim = 0; dim < var.dim(); ++dim)
			for (std::size_t dof = 0; dof < n_dofs; ++dof)
				values(dim, dof) = solution_[dofs[dof].index + dim];

		return values;
	}

	template<class T>
	auto operator[](const T& ref) const
	{
		return at(ref);
	}

	Value at(const Point2& pt, const typename Mesh::Cell_view& cell) const
	{
		static_assert(internal::is_dim2<Mesh>);

		const auto pt_ref = point_to_ref_triangle(pt, cell);
		const auto dofs = esf::dofs(system_, cell, var_index);
		Value value{};
		for (Local_index id = 0; id < dofs.size(); ++id)
			value += Element::basis(id, pt_ref) * solution_[dofs[id].index];
		return value;
	}

	Value operator()(const Point2& pt, const typename Mesh::Cell_view& cell) const
	{
		return at(pt, cell);
	}

	const System& system() const
	{
		return system_;
	}

	const Mesh& mesh() const
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
auto at_quadr(const Solution_view<System, var, Value>& solution,
			  const typename System::Mesh::Cell_view& cell)
{
	return solution.template at_quadr<Quadr>(cell);
}
} // namespace esf
