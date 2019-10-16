#include <esl/sparse/utility/mkl_helpers.hpp>

#include <mkl_spblas.h>

#include <string>

namespace esl::internal
{
std::string mkl_sparse_status_string(::sparse_status_t status)
{
	switch (status)
	{
	case ::sparse_status_t::SPARSE_STATUS_ALLOC_FAILED:
		return "Internal memory allocation failed";

	case ::sparse_status_t::SPARSE_STATUS_EXECUTION_FAILED:
		return "Execution failed";

	case ::sparse_status_t::SPARSE_STATUS_INTERNAL_ERROR:
		return "An error in algorithm implementation occurred";

	case ::sparse_status_t::SPARSE_STATUS_INVALID_VALUE:
		return "The input parameters contain an invalid value";

	case ::sparse_status_t::SPARSE_STATUS_NOT_INITIALIZED:
		return "The routine encountered an empty handle or matrix array";

	case ::sparse_status_t::SPARSE_STATUS_NOT_SUPPORTED:
		return "The requested operation is not supported";

	case ::sparse_status_t::SPARSE_STATUS_SUCCESS:
		return "The operation was successful";
	}

	return "Unknown error";
}
} // namespace esl::internal
