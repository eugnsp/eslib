#pragma once
#include <esf/types.hpp>
#include <esf/util/algorithm.hpp>

#include <esl/sparse/sparsity_pattern.hpp>

#include <algorithm>
#include <cassert>
#include <cstddef>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <utility>
#include <vector>

namespace esf::internal
{
template<class Symmetry_tag, class System>
esl::Sparsity_pattern<Symmetry_tag> sparsity_pattern(const System& system)
{
	const auto& dof_mapper = system.dof_mapper();
	esl::Sparsity_pattern<Symmetry_tag> pattern(dof_mapper.n_free_dofs());

	std::vector<std::size_t> indices, cols;

	for (auto& cell : system.mesh().cells())
	{
		const auto dofs_list = dof_mapper.all_dofs(cell);
		indices.clear();
		esf::for_each_var<typename System::Var_list>([&](auto var) {
			const auto& dofs = std::get<var>(dofs_list);
			const auto n_dofs = static_cast<esf::Local_index>(dofs.size());

			for (esf::Local_index i = 0; i < n_dofs; ++i)
				if (dofs[i].is_free)
					for (esf::Index d = 0; d < system.variable(var).dim(); ++d)
						indices.push_back(dofs[i].index + d * n_dofs);
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
} // namespace esf::internal
