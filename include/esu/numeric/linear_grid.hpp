#pragma once
#include <cassert>
#include <cstddef>

namespace esu
{
// 1D evenly spaced grid
template<class T>
class Linear_grid
{
public:
	Linear_grid() = default;

	static Linear_grid from_min_max(T x_min, T x_max, std::size_t size)
	{
		const auto dx = (x_max - x_min) / size;
		return Linear_grid{x_min, dx, size};
	}

	static Linear_grid from_min_step(T x_min, T step, std::size_t size)
	{
		return Linear_grid{x_min, step, size};
	}

	explicit Linear_grid(T x0)
		: x0_(x0), dx_(0), size_(1)
	{ }

	void set_x0_dx_size(T x0, T dx, std::size_t size)
	{
		assert(size > 0);
		x0_ = x0;
		dx_ = dx;
		size_ = size;
	}

	void operator+=(T dx)
	{
		x0_ += dx;
	}

	T operator[](std::size_t index) const
	{
		assert(index < size_);
		return x0_ + index * dx_;
	}

	std::size_t size() const
	{
		return size_;
	}

	T dx() const
	{
		return dx_;
	}

	T range() const
	{
		return dx_ * (size_ - 1);
	}

	T front() const
	{
		return x0_;
	}

	T back() const
	{
		return front() + range();
	}

private:
	Linear_grid(T x0, T dx, std::size_t size)
		: x0_(x0), dx_(dx), size_(size)
	{
		assert(size > 0);
	}

private:
	T x0_;
	T dx_;
	std::size_t size_;
};

// template<typename T>
// Linear_grid<T> operator+(Linear_grid<T> grid, T offset)
// {
// 	return (grid += offset);
// }
//
// template<typename T>
// Linear_grid<T> operator+(T offset, const Linear_grid<T>& grid)
// {
// 	return grid + offset;
// }
//
// template<typename T>
// Linear_grid<T> operator-(Linear_grid<T> grid, T offset)
// {
// 	return (grid -= offset);
// }
}
