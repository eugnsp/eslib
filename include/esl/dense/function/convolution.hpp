#pragma once
#include <esl/dense/dense.hpp>
#include <esl/dense/matrix.hpp>
#include <esl/dense/type_traits.hpp>

#include <mkl_types.h>
#include <mkl_vsl.h>

#include <cmath>
#include <cstddef>
#include <optional>
#include <stdexcept>
#include <string>

#define ESL_CALL_MKL_VSL(fn, ...)                                                                                    \
	do                                                                                                                 \
	{                                                                                                                  \
		[[maybe_unused]] const auto status = fn(__VA_ARGS__);                                                          \
		if (status != VSL_STATUS_OK)                                                                                   \
			throw std::runtime_error("Exception in " #fn ": " + internal::mkl_vsl_status_string(status));              \
	} while (0)

namespace esl
{
namespace internal
{
std::string mkl_vsl_status_string(MKL_INT);
} // namespace internal

template<class Expr1, class Expr2>
void cols_convolution(const Dense<Expr1, Lvalue>& x, Dense<Expr2, Lvalue>& y, std::optional<std::size_t> y_start = {})
{
	static_assert(internal::have_same_value_types<Expr1, Expr2>, "Incompatible data types");
	static_assert(internal::is_vector<Expr1>, "Expression should be a vector");

	::VSLConvTaskPtr task;
	ESL_CALL_MKL_VSL(::vsldConvNewTaskX1D, &task, VSL_CONV_MODE_AUTO, x.size(), y.rows(), y.rows(), x.self().data(),
		x.self().row_stride());

	if (y_start)
	{
		const auto start = static_cast<MKL_INT>(*y_start);
		ESL_CALL_MKL_VSL(::vslConvSetStart, task, &start);
	}

	esl::Vector_xd z(y.rows());
	for (std::size_t col = 0; col < y.cols(); ++col)
	{
		auto yc = y.col_view(col);
		ESL_CALL_MKL_VSL(::vsldConvExecX1D, task, yc.data(), yc.row_stride(), z.data(), 1);
		yc = z;
	}

	ESL_CALL_MKL_VSL(::vslConvDeleteTask, &task);
}

template<class Expr1, class Expr2>
void rows_convolution(const Dense<Expr1, Lvalue>& x, Dense<Expr2, Lvalue>& y, std::optional<std::size_t> y_start = {})
{
	static_assert(internal::have_same_value_types<Expr1, Expr2>, "Incompatible data types");
	static_assert(internal::is_vector<Expr1>, "Expression should be a vector");

	::VSLConvTaskPtr task;
	ESL_CALL_MKL_VSL(::vsldConvNewTaskX1D, &task, VSL_CONV_MODE_AUTO, x.size(), y.cols(), y.cols(), x.self().data(),
		x.self().row_stride());

	if (y_start)
	{
		const auto start = static_cast<MKL_INT>(*y_start);
		ESL_CALL_MKL_VSL(::vslConvSetStart, task, &start);
	}

	esl::Vector_xd z(y.cols());
	for (std::size_t row = 0; row < y.rows(); ++row)
	{
		auto yr = y.row_view(row).tr_view();
		ESL_CALL_MKL_VSL(::vsldConvExecX1D, task, yr.data(), yr.row_stride(), z.data(), 1);
		yr = z;
	}

	ESL_CALL_MKL_VSL(::vslConvDeleteTask, &task);
}
} // namespace esl
