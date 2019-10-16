#include <mkl_service.h>

namespace esl::internal
{
[[gnu::destructor]] void mkl_destructor()
{
	::mkl_free_buffers();
}
} // namespace esl::internal
