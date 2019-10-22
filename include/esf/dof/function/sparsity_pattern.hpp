#pragma once
#include <esf/dof/function.hpp>
#include <esf/index.hpp>
#include <esf/utility/system_for_each.hpp>
#include <esf/var.hpp>

#include <esl/sparse/sparsity_pattern.hpp>

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

namespace esf
{
namespace internal
{
template<
	class Symmetry_tag,
	class System>
esl::Sparsity_pattern<Symmetry_tag> sparsity_pattern(const System& system)
{
	esl::Sparsity_pattern<Symmetry_tag> pattern(system.dof_mapper().n_free_dofs());
	std::vector<std::size_t> indices, cols;

	for (auto& cell : system.mesh().cells())
	{
		indices.clear();
		esf::for_each_variable(system,
			[&]<std::size_t var_idx, class Var>(Var_index<var_idx> var_index, const Var& var)
		{
			const auto dofs = esf::dofs(system, cell, var_index);
			for (std::size_t i = 0; i < dofs.size(); ++i)
				if (dofs[i].is_free)
					for (Index d = 0; d < var.dim(); ++d)
						indices.push_back(dofs[i].index + d * dofs.size());
		});

		for (std::size_t i = 0; i < indices.size(); ++i)
		{
			cols.clear();
			for (std::size_t j = 0; j < indices.size(); ++j)
			{
				if constexpr (std::is_same_v<Symmetry_tag, esl::Symmetric_upper>)
				{
					if (indices[j] >= indices[i])
						cols.push_back(indices[j]);
				}
				else
					cols.push_back(indices[j]);
			}
			std::sort(cols.begin(), cols.end());
			pattern.insert(indices[i], cols.begin(), cols.end());
		}
	}
#ifndef NDEBUG
	for (std::size_t r = 0; r < pattern.n_rows(); ++r)
	{
		auto& row = pattern.row(r);
		if (row.empty())
			throw std::runtime_error("Row " + std::to_string(r) + " is empty");
	}
	assert(pattern.is_each_row_not_empty());
	auto err = pattern.check();
	if (err)
		throw std::runtime_error(err.message());
#endif
	return pattern;
}
} // namespace internal

template<
	class Sparse_matrix,
	class System>
void compute_and_set_sparsity_pattern(
	const System&  system,
	Sparse_matrix& matrix)
{
	using Symmetry = typename Sparse_matrix::Symmetry_tag;

	auto pattern = system.template sparsity_pattern<Symmetry>();
	matrix.set_sparsity_pattern(std::move(pattern));
}

template<
	class Sparse_matrix,
	class System,
	class Coupling_fn>
void compute_and_set_sparsity_pattern(
	const System&  system,
	Coupling_fn&&  coupling_fn,
	Sparse_matrix& matrix)
{
	using Symmetry = typename Sparse_matrix::Symmetry_tag;

	auto pattern =
		system.template sparsity_pattern<Symmetry>(std::forward<Coupling_fn>(coupling_fn));
	matrix.set_sparsity_pattern(std::move(pattern));
}

template<
	class Sparse_matrix,
	class System,
	class Coupling_fn>
void compute_and_set_sparsity_pattern2(
	const System&  system,
	Coupling_fn&&  coupling_fn,
	Sparse_matrix& matrix)
{
	using Symmetry = typename Sparse_matrix::Symmetry_tag;

	auto pattern =
		system.template sparsity_pattern2<Symmetry>(std::forward<Coupling_fn>(coupling_fn));
	matrix.set_sparsity_pattern(std::move(pattern));
}
} // namespace esf
