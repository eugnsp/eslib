#pragma once
#include <esf/geometry/compare.hpp>
#include <esf/geometry/point1.hpp>

#include <esu/algorithm.hpp>

#include <cassert>
#include <cmath>
#include <cstddef>
#include <vector>

namespace esf
{
class Linear_grid
{
public:
	std::size_t size() const
	{
		return ticks_.back().n + 1;
	}

	void add_tick(double coord)
	{
		assert(ticks_.empty());

		ticks_.push_back({0, coord, 0});
	}

	void add_tick(std::size_t n, double coord, double bowing = 0)
	{
		assert(!ticks_.empty());
		assert(n > ticks_.back().n);
		assert(coord > ticks_.back().coord);
		assert(std::abs(bowing) < 1);

		ticks_.push_back({n, coord, bowing});
	}

	std::vector<esf::Point1> grid() const
	{
		std::vector<esf::Point1> grid(ticks_.back().n + 1);

		grid.front().x() = ticks_.front().coord;
		esu::for_each_pair(ticks_.begin(), ticks_.end(),
			[&grid](auto& left, auto& right) { make_grid_on_interval(grid, left, right); });

		return grid;
	}

private:
	struct Tick
	{
		const std::size_t n;
		const double coord;
		const double bowing;
	};

	static void make_grid_on_interval(
		std::vector<esf::Point1>& grid, const Tick& left, const Tick& right)
	{
		if (right.bowing == 0)
		{
			const double d = (right.coord - left.coord) / (right.n - left.n);
			assert(!is_geom_equal(d, 0));

			for (auto j = left.n + 1; j <= right.n; ++j)
				grid[j].x() = left.coord + d * (j - left.n);
		}
		else
		{
			const double c = (1 + right.bowing) / (1 - right.bowing);
			const double d = (right.coord - left.coord) / (c - 1);
			assert(!is_geom_equal(d, 0));

			for (auto j = left.n + 1; j <= right.n; ++j)
			{
				const auto p = static_cast<double>(j - left.n) / (right.n - left.n);
				grid[j].x() = left.coord + d * (std::pow(c, p) - 1);
			}
		}
	}

private:
	std::vector<Tick> ticks_;
};
} // namespace esf
