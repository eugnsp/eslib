#include <esl/sparse/function/check.hpp>

namespace esl
{
Csr_matrix_check_error_code::Csr_matrix_check_error_code(
	::sparse_checker_error_values error_code, Check_result check_result) :
	error_code_(error_code),
	check_result_(check_result)
{}

Csr_matrix_check_error_code::operator bool() const
{
	return error_code_ != ::sparse_checker_error_values::MKL_SPARSE_CHECKER_SUCCESS;
}

std::string Csr_matrix_check_error_code::to_string() const
{
	switch (error_code_)
	{
	case ::sparse_checker_error_values::MKL_SPARSE_CHECKER_SUCCESS:
		return "No error";

	case ::sparse_checker_error_values::MKL_SPARSE_CHECKER_NON_MONOTONIC:
		return "The input array is not 0 or 1 based or elements of ia "
			   "are not in non-decreasing order as required: " +
			   check_result_string_non_monotonic();

	case ::sparse_checker_error_values::MKL_SPARSE_CHECKER_OUT_OF_RANGE:
		return "The value of the ja array is lower than the number of the first column "
			   "or greater than the number of the last column: " +
			   check_result_string_non_monotonic();

	case ::sparse_checker_error_values::MKL_SPARSE_CHECKER_NONTRIANGULAR:
		return "The matrix_structure parameter is MKL_UPPER_TRIANGULAR and both ia and ja "
			   "are not upper triangular, or the matrix_structure parameter is "
			   "MKL_LOWER_TRIANGULAR and both ia and ja are not lower triangular: " +
			   check_result_string_non_triag();

	case ::sparse_checker_error_values::MKL_SPARSE_CHECKER_NONORDERED:
		return "The elements of the ja array are not in non-decreasing order "
			   "in each row as required: " +
			   check_result_string_non_ordered();

	default:
		return "Unknown error";
	}
}

::sparse_checker_error_values Csr_matrix_check_error_code::error_code() const
{
	return error_code_;
}

auto Csr_matrix_check_error_code::check_result() const -> Check_result
{
	return check_result_;
}

std::string Csr_matrix_check_error_code::check_result_string_non_monotonic() const
{
	const auto i = std::to_string(check_result_[0]);
	const auto ai = std::to_string(check_result_[1]);
	const auto aip1 = std::to_string(check_result_[2]);

	return "i=" + i + ", ia[i]=" + ai + " and ia[i+1]=" + aip1 + " are incompatible";
}

std::string Csr_matrix_check_error_code::check_result_string_non_triag() const
{
	const auto i = std::to_string(check_result_[0]);
	const auto j = std::to_string(check_result_[1]);
	const auto jai = std::to_string(check_result_[2]);

	return "i=" + i + ", j=" + j + ", ia[i]=j=" + j + " and ja[j]=" + jai + " are incompatible";
}

std::string Csr_matrix_check_error_code::check_result_string_non_ordered() const
{
	const auto j = std::to_string(check_result_[0]);
	const auto aj = std::to_string(check_result_[1]);
	const auto ajp1 = std::to_string(check_result_[2]);

	return "j=" + j + ", ja[j]=" + aj + " and ja[j+1]=" + ajp1 + " are incompatible";
}
} // namespace esl
