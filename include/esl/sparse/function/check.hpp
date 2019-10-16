#pragma once
#include <esl/sparse/csr_matrix.hpp>

#include <esu/array.hpp>

#include <mkl_sparse_handle.h>
#include <mkl_spblas.h>
#include <mkl_types.h>

#include <array>
#include <cassert>
#include <string>
#include <type_traits>

namespace esl
{
class Csr_matrix_check_error_code
{
private:
	using Check_result = decltype(esu::to_array(::sparse_struct::check_result));

public:
	Csr_matrix_check_error_code(::sparse_checker_error_values, Check_result);

	explicit operator bool() const;
	std::string to_string() const;

	::sparse_checker_error_values error_code() const;
	Check_result check_result() const;

private:
	std::string check_result_string_non_monotonic() const;
	std::string check_result_string_non_triag() const;
	std::string check_result_string_non_ordered() const;

private:
	const ::sparse_checker_error_values error_code_;
	const Check_result check_result_;
};

template<typename Value, class Symmetry_tag>
Csr_matrix_check_error_code check(const Csr_matrix<Value, Symmetry_tag, MKL_UINT>& matrix, bool print_error = false)
{
	assert(matrix.rows() == matrix.cols());

	::sparse_struct handle;

	::sparse_matrix_checker_init(&handle);
	handle.n = static_cast<MKL_INT>(matrix.rows());
	handle.csr_ia = const_cast<MKL_INT*>(reinterpret_cast<const MKL_INT*>(matrix.row_indices()));
	handle.csr_ja = const_cast<MKL_INT*>(reinterpret_cast<const MKL_INT*>(matrix.col_indices()));
	handle.indexing = ::sparse_matrix_indexing::MKL_ZERO_BASED;
	handle.matrix_format = ::sparse_matrix_formats::MKL_CSR;
	handle.print_style = ::sparse_matrix_print_styles::MKL_C_STYLE;
	handle.message_level =
		print_error ? ::sparse_matrix_message_levels::MKL_PRINT : ::sparse_matrix_message_levels::MKL_NO_PRINT;

	if constexpr (std::is_same_v<Symmetry_tag, Symmetric_upper>)
		handle.matrix_structure = ::sparse_matrix_structures::MKL_UPPER_TRIANGULAR;
	else if constexpr (std::is_same_v<Symmetry_tag, Symmetric_lower>)
		handle.matrix_structure = ::sparse_matrix_structures::MKL_LOWER_TRIANGULAR;
	else if constexpr (std::is_same_v<Symmetry_tag, Structural_symmetric>)
		handle.matrix_structure = ::sparse_matrix_structures::MKL_STRUCTURAL_SYMMETRIC;
	else
		handle.matrix_structure = ::sparse_matrix_structures::MKL_GENERAL_STRUCTURE;

	const auto error_code = ::sparse_matrix_checker(&handle);
	return {error_code, esu::to_array(handle.check_result)};
}
} // namespace esl
