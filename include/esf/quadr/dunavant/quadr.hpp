#pragma once
#include <esf/index.hpp>
#include <esf/quadr/dunavant/data.hpp>

#include <esl/dense.hpp>
#include <esu/array.hpp>

#include <cassert>
#include <tuple>
#include <utility>

namespace esf
{
namespace internal
{
// Dunavant quadrature integration
template<Index order, class Group_index_sequence>
class Dunavant_quadr;

template<Index order, Index... group_indices>
class Dunavant_quadr<order, std::integer_sequence<Index, group_indices...>>
{
private:
	static constexpr auto point_groups = Dunavant_data<order>::groups;
	static constexpr auto group_sizes =
		esu::make_array(std::get<group_indices>(point_groups).size()...);

public:
	static constexpr Index dim = 2;

	// The total number of quadrature points
	static constexpr Index size = esu::array_sum<Index>(group_sizes, 0);

public:
	// Computes a weighted sum over quadrature points,
	// sum_i weight(i) * fn(i) for i = 0, ... size() - 1
	template<class Fn>
	static constexpr auto sum(Fn fn)
	{
		return (... + std::get<group_indices>(point_groups).sum(fn, start_index<group_indices>()));
	}

protected:
	// Returns an array of all quadrature points
	static constexpr auto points()
	{
		return esu::array_cat(std::get<group_indices>(point_groups).points()...);
	}

private:
	// Returns the index of the first point in the group with the given index
	template<Index group_index>
	static constexpr Index start_index()
	{
		return esu::array_sum_n<group_index, Index>(group_sizes, 0);
	}
};

template<Index order>
using Dunavant_quadr_t = Dunavant_quadr<order,
	std::make_integer_sequence<Index, std::tuple_size_v<decltype(Dunavant_data<order>::groups)>>>;
} // namespace internal

// Quadrature integration
template<Index order>
class Quadr<order, 2> : public internal::Dunavant_quadr_t<order>
{
private:
	using Base = internal::Dunavant_quadr_t<order>;

public:
	static constexpr Index dim = 2;

public:
	// Returns a quadrature point with the given index in the reference triangle
	static constexpr esl::Vector_2d point(Index i)
	{
		constexpr auto points = Base::points();
		assert(i < points.size());
		return points[i];
	}
};
} // namespace esf
