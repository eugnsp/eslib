#pragma once
#include <esf/types.hpp>

#include <esl/dense.hpp>

#include <array>

namespace esf::internal
{
// Group of Dunavant quadrature points; a group contains
// weight and 1, 3 or 6 points that are obtained via all possible
// permutations of 1, 2 or 3 barycentric coordinates
template<Local_index n>
class Dunavant_points_group
{
	static_assert(1 <= n && n <= 3);

public:
	template<typename... Coords>
	constexpr Dunavant_points_group(double weight, Coords... coords) :
		weight_(weight), coords_{coords...}
	{
		static_assert(sizeof...(Coords) == n);
	}

	// Returns the number of points in the group
	static constexpr Local_index size()
	{
		return n * (n + 1) / 2;
	}

	constexpr std::array<esl::Vector_2d, size()> points() const
	{
		using V = esl::Vector_2d;

		if constexpr (n == 1)
			return {V{coords_[0], coords_[0]}};
		else if constexpr (n == 2)
			return {V{coords_[0], coords_[1]}, V{coords_[1], coords_[0]},
				V{coords_[1], coords_[1]}};
		else if constexpr (n == 3)
			return {V{coords_[0], coords_[1]}, V{coords_[0], coords_[2]},
				V{coords_[1], coords_[2]}, V{coords_[1], coords_[0]},
				V{coords_[2], coords_[0]}, V{coords_[2], coords_[1]}};
	}

	// Computes the sum (weight) * (fn(start_index) + fn(start_index + 1) + ...)
	// over all points in the group
	template<class Fn>
	constexpr auto sum(Fn fn, Local_index start_index) const
	{
		return weight_ * sum_impl(fn, start_index, Make_local_index_sequence<size()>{});
	}

private:
	template<class Fn, Local_index... indices>
	static constexpr auto sum_impl(Fn fn, Local_index start_index, Local_index_sequence<indices...>)
	{
		return (... + fn(start_index + indices));
	}

private:
	const double weight_;
	const std::array<double, n> coords_;
};

template<typename... Coords>
Dunavant_points_group(double, Coords...)->Dunavant_points_group<sizeof...(Coords)>;
} // namespace esf::internal
