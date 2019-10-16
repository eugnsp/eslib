#include <esl/dense/function/convolution.hpp>

#include <mkl_types.h>
#include <mkl_vsl.h>

#include <string>

namespace esl::internal
{
std::string mkl_vsl_status_string(MKL_INT status)
{
	switch (status)
	{
	case VSL_STATUS_OK:
		return "No error";

	case VSL_CC_ERROR_NOT_IMPLEMENTED:
		return "Requested functionality is not implemented";

	case VSL_CC_ERROR_ALLOCATION_FAILURE:
		return "Memory allocation failure";

	case VSL_CC_ERROR_BAD_DESCRIPTOR:
		return "Task descriptor is corrupted";

	case VSL_CC_ERROR_SERVICE_FAILURE:
		return "A service function has failed";

	case VSL_CC_ERROR_EDIT_FAILURE:
		return "Failure while editing the task";

	case VSL_CC_ERROR_EDIT_PROHIBITED:
		return "You cannot edit this parameter";

	case VSL_CC_ERROR_COMMIT_FAILURE:
		return "Task commitment has failed";

	case VSL_CC_ERROR_COPY_FAILURE:
		return "Failure while copying the task";

	case VSL_CC_ERROR_DELETE_FAILURE:
		return "Failure while deleting the task";

	case VSL_CC_ERROR_BAD_ARGUMENT:
		return "Bad argument or task parameter";

	case VSL_CC_ERROR_JOB:
		return "Bad parameter: job";

	case VSL_CC_ERROR_KIND:
		return "Bad parameter: kind";

	case VSL_CC_ERROR_MODE:
		return "Bad parameter: mode";

	case VSL_CC_ERROR_METHOD:
		return "Bad parameter: method";

	case VSL_CC_ERROR_TYPE:
		return "Bad parameter: type";

	case VSL_CC_ERROR_EXTERNAL_PRECISION:
		return "Bad parameter: external_precision";

	case VSL_CC_ERROR_INTERNAL_PRECISION:
		return "Bad parameter: internal_precision";

		// MKL bug: duplicated value, see
		// https://software.intel.com/en-us/forums/intel-math-kernel-library/topic/810189
		// case VSL_CC_ERROR_PRECISION:
		// 	return "Incompatible external/internal precisions";

	case VSL_CC_ERROR_DIMS:
		return "Bad parameter: dims";

	case VSL_CC_ERROR_XSHAPE:
		return "Bad parameter: xshape";

	case VSL_CC_ERROR_YSHAPE:
		return "Bad parameter: yshape";

	case VSL_CC_ERROR_ZSHAPE:
		return "Bad parameter: zshape";

	case VSL_CC_ERROR_XSTRIDE:
		return "Bad parameter: xstride";

	case VSL_CC_ERROR_YSTRIDE:
		return "Bad parameter: ystride";

	case VSL_CC_ERROR_ZSTRIDE:
		return "Bad parameter: zstride";

	case VSL_CC_ERROR_X:
		return "Bad parameter: x";

	case VSL_CC_ERROR_Y:
		return "Bad parameter: y";

	case VSL_CC_ERROR_Z:
		return "Bad parameter: z";

	case VSL_CC_ERROR_START:
		return "Bad parameter: start";

	case VSL_CC_ERROR_DECIMATION:
		return "Bad parameter: decimation";

	case VSL_CC_ERROR_OTHER:
		return "Another error";

	default:
		return "Unknown error";
	}
}
} // namespace esl::internal
