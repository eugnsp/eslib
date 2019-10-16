#pragma once
#include <esl/dense/type_traits.hpp>
#include <esl/dense/utility/ct_extent.hpp>

#include <cassert>
#include <utility>

namespace esl::internal
{
template<class Expr1, class Expr2>
class Mul_fn
{
	static_assert(is_extent_dynamic_or_eq(ct_cols_value<Expr1>, ct_rows_value<Expr2>), "Incompatible extents");

public:
	using Value = decltype(std::declval<Value_type<Expr1>>() * std::declval<Value_type<Expr2>>());

	static constexpr auto ct_rows = ct_rows_value<Expr1>;
	static constexpr auto ct_cols = ct_cols_value<Expr2>;

public:
	static void check_sizes([[maybe_unused]] const Expr1& expr1, [[maybe_unused]] const Expr2& expr2)
	{
		assert(expr1.cols() == expr2.rows());
	}

	static std::size_t rows(const Expr1& expr1, const Expr2&)
	{
		return expr1.rows();
	}

	static std::size_t cols(const Expr1&, const Expr2& expr2)
	{
		return expr2.cols();
	}

	static Value element(const Expr1& expr1, const Expr2& expr2, std::size_t row, std::size_t col)
	{
		auto result = Value{};
		for (std::size_t i = 0; i < expr1.cols(); ++i)
			result += expr1(row, i) * expr2(i, col);

		return result;
	}
};
} // namespace esl::internal
