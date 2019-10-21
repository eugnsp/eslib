#pragma once
#include <esf/forward.hpp>
#include <esf/index.hpp>
#include <esf/quadr/dunavant/data.hpp>
#include <esf/tags.hpp>

#include <esl/dense.hpp>
#include <esu/array.hpp>

#include <cassert>
#include <cstddef>
#include <tuple>
#include <utility>

namespace esf
{
namespace internal
{
// Dunavant quadrature integration
template<std::size_t order, class Group_index_sequence>
class Dunavant_quadr;

template<std::size_t order, std::size_t... group_indices>
class Dunavant_quadr<order, std::index_sequence<group_indices...>>
{
private:
	static constexpr auto point_groups = Dunavant_data<order>::groups;
	static constexpr auto group_sizes =
		esu::make_array(std::get<group_indices>(point_groups).size()...);

public:
	// The total number of quadrature points
	static constexpr std::size_t size = esu::array_sum(group_sizes);

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
	template<std::size_t group_index>
	static constexpr std::size_t start_index()
	{
		return esu::array_sum_n<group_index>(group_sizes);
	}
};

template<std::size_t order>
using Dunavant_quadr_t = Dunavant_quadr<order,
	std::make_index_sequence<std::tuple_size_v<decltype(Dunavant_data<order>::groups)>>>;
} // namespace internal

// Quadrature integration
template<std::size_t order>
class Quadr<order, Dim2> : public internal::Dunavant_quadr_t<order>
{
private:
	using Base = internal::Dunavant_quadr_t<order>;

public:
	using Space_dim = Dim2;

public:
	// Returns a quadrature point with the given index in the reference triangle
	static constexpr esl::Vector_2d point(std::size_t i)
	{
		constexpr auto points = Base::points();
		assert(i < points.size());
		return points[i];
	}
};
} // namespace esf
