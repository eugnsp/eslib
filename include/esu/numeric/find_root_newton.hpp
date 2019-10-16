#pragma once
#include <cassert>
#include <cmath>
#include <stdexcept>

namespace esu
{
// Finds a root using the Newton-Raphson method
template<typename T, class Fn>
T find_root_newton(Fn fn, T x, T x_acc, unsigned int max_n_its = 20)
{
	assert(x_acc > 0);

	for (unsigned int i = 0; i < max_n_its; ++i)
	{
		const auto [f, df] = fn(x);
		const auto dx = f / df;
		x -= dx;

		if (std::abs(dx) < x_acc)
			return x;
	}

	throw std::runtime_error("Too many iterations in find_root_newton");
}
}
