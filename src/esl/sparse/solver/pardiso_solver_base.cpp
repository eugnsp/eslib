#include <esl/sparse/solver/pardiso_solver_base.hpp>

#include <mkl_types.h>

#include <string>

namespace esl::internal
{
std::string Pardiso_solver_base::pardiso_error_string(MKL_INT error)
{
	std::string str = "PARDISO error " + std::to_string(error) + ": ";
	switch (error)
	{
	case 0:
		return str + "No error";
	case -1:
		return str + "Input inconsistent";
	case -2:
		return str + "Not enough memory";
	case -3:
		return str + "Reordering problem";
	case -4:
		return str + "Zero pivot, numerical factorization or iterative refinement problem";
	case -5:
		return str + "Unclassified (internal) error";
	case -6:
		return str + "Reordering failed (nonsymmetric real and complex matrices only)";
	case -7:
		return str + "Diagonal matrix is singular";
	case -8:
		return str + "32-bit integer overflow problem";
	case -9:
		return str + "Not enough memory for OOC";
	case -10:
		return str + "Error opening OOC files";
	case -11:
		return str + "Read/write error with OOC files";
	case -12:
		return str + "pardiso_64 called from 32-bit library";
	}

	return str + "Unknown error";
}
} // namespace esl::internal
