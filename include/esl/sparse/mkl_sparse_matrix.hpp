#pragma once
#include <esl/dense/expression.hpp>
#include <esl/dense/tags.hpp>
#include <esl/dense/type_traits.hpp>
#include <esl/sparse/forward.hpp>
#include <esl/sparse/type_traits.hpp>
#include <esl/sparse/utility/mkl_helpers.hpp>
#include <esl/sparse/utility/mkl_overloads.hpp>

#include <esu/type_traits.hpp>

#include <mkl_spblas.h>
#include <mkl_types.h>

#include <cassert>
#include <stdexcept>
#include <type_traits>
#include <utility>

#define ESL_CALL_MKL_SPARSE(fn, ...)                                                                                 \
	do                                                                                                                 \
	{                                                                                                                  \
		[[maybe_unused]] const auto status = fn(__VA_ARGS__);                                                          \
		if (status != ::sparse_status_t::SPARSE_STATUS_SUCCESS)                                                        \
			throw std::runtime_error("Exception in " #fn ": " + internal::mkl_sparse_status_string(status));           \
	} while (0)

namespace esl
{
template<class Matrix>
class Mkl_sparse_matrix : public Expression<Mkl_sparse_matrix<Matrix>>
{
public:
	using Index = typename Matrix::Index;
	using Value = typename Matrix::Value;

	static_assert(internal::is_csr_matrix<Matrix>, "Bad matrix type");
	static_assert(sizeof(Index) == sizeof(MKL_INT), "Incompatible matrix index type size");
	static_assert(internal::is_fd_or_cfd<Value>, "Incompatible matrix element type");

public:
	Mkl_sparse_matrix(const Mkl_sparse_matrix&) = delete;
	Mkl_sparse_matrix& operator=(const Mkl_sparse_matrix&) = delete;

	Mkl_sparse_matrix(Mkl_sparse_matrix&& other)
	{
		std::swap(handle_, other.handle_);
	}

	Mkl_sparse_matrix& operator=(Mkl_sparse_matrix&& other)
	{
		std::swap(handle_, other.handle_);
		return *this;
	}

	~Mkl_sparse_matrix()
	{
		if (handle_ != nullptr)
		{
			[[maybe_unused]] const auto status = ::mkl_sparse_destroy(handle_);
			assert(status == ::SPARSE_STATUS_SUCCESS);
		}
	}

	///////////////////////////////////////////////////////////////////////
	//* Extents */

	Index rows() const
	{
		return matrix_size().first;
	}

	Index cols() const
	{
		return matrix_size().second;
	}

	///////////////////////////////////////////////////////////////////////
	//* Native MKL handle */

	::sparse_matrix_t handle()
	{
		return handle_;
	}

	::sparse_matrix_t handle() const
	{
		return handle_;
	}

	////////////////////////////////////////////////////////////////////////
	//* Modifiers */

	void set(const Index row, const Index col, const Value value)
	{
		static_assert(!std::is_const_v<Matrix>, "Not a writable matrix");

		ESL_CALL_MKL_SPARSE(internal::mkl_sparse_set_value, handle_, row, col, value);
	}

	void order()
	{
		static_assert(!std::is_const_v<Matrix>, "Not a writable matrix");
		static_assert(internal::is_csr_matrix<Matrix>, "Not a CSR matrix");

		ESL_CALL_MKL_SPARSE(::mkl_sparse_order, handle_);
	}

protected:
	Mkl_sparse_matrix() = default;

private:
	std::pair<Index, Index> matrix_size() const
	{
		if constexpr (internal::is_csr_matrix<Matrix>)
		{
			sparse_index_base_t indexing;
			MKL_UINT rows, cols, *rows_start, *rows_end, *col_index;
			Value* values;

			ESL_CALL_MKL_SPARSE(internal::mkl_sparse_export_csr, handle_, indexing, rows, cols, rows_start, rows_end,
				col_index, values);
			return {static_cast<Index>(rows), static_cast<Index>(cols)};
		}
		else
			static_assert(esu::always_false<Matrix>);
	}

protected:
	::sparse_matrix_t handle_ = nullptr;
};

//////////////////////////////////////////////////////////////////////
//> Type traits

template<typename Matrix>
struct Traits<Mkl_sparse_matrix<Matrix>>
{
	using Value = Value_type<Matrix>;
	using Layout = No_layout;

	static constexpr std::size_t rows = dynamic;
	static constexpr std::size_t cols = dynamic;
};
} // namespace esl
