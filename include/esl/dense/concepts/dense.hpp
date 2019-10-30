#pragma once
#include <cstddef>

namespace esl::concepts
{
template<class Expr>
concept dense = requires(Expr expr)
{
	typename Expr::Value;

	{ expr.size() } -> std::size_t;
	{ expr.rows() } -> std::size_t;
	{ expr.cols() } -> std::size_t;
	{ expr(0, 0) } -> typename Expr::Value;
};
}
